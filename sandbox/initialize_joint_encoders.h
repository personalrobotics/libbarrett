/**
 * @file custom_main_function.h
 *
 * Creates communications server on startup and uses socket communications to
 * wait for signals to zero the robot.
 */

#ifndef INITIALIZE_JOINT_ENCODERS_H_
#define INITIALIZE_JOINT_ENCODERS_H_

#include <barrett/products/product_manager.h>
#include <barrett/systems/wam.h>
#include <barrett/units.h>
#include "custom_messages.h"
#include "socket_communications.h"


// Check if joint encoders have been initialized.
template<size_t DOF>
bool jointEncodersInitialized(
    barrett::systems::Wam<DOF> *wam, const size_t kNumJenc) {
  bool initialized = true;
  for (size_t i = 0; i < kNumJenc; ++i) {
    initialized &= wam->getLowLevelWam().getMotorPucks()[i].foundIndexPulse();
  }
  return initialized;
}

// Reset encoder correction values (ECMIN and ECMAX).
void resetEncoderCorrections(barrett::ProductManager *pm) {
  for (size_t i = 0; i < pm->getWamPucks().size(); ++i) {
    if (pm->getWamPucks()[i] != NULL) {
      pm->getWamPucks()[i]->setProperty(barrett::Puck::ECMIN, 0);
      pm->getWamPucks()[i]->setProperty(barrett::Puck::ECMAX, 0);
    }
  }
}

// Check if ECMIN and ECMAX are within reasonable bounds. Typically we expect
// them to be around 35-45, but 200 is a conservative estimate.
bool checkEncoderCorrections(barrett::ProductManager *pm, uint bound = 200) {
  for (size_t i = 0; i < pm->getWamPucks().size(); ++i) {
    if (pm->getWamPucks()[i] != NULL) {
      uint temp1 = abs(pm->getWamPucks()[i]->getProperty(barrett::Puck::ECMIN));
      barrett::btsleep(0.005);
      uint temp2 = abs(pm->getWamPucks()[i]->getProperty(barrett::Puck::ECMAX));
      barrett::btsleep(0.005);
      if ((temp1 > bound) || (temp2 > bound)) {
        return false;
      }
    }
  }
  return true;
}

void monitorPendantState(barrett::SafetyModule *sm, bool *done,
                         barrett::SafetyModule::SafetyMode *mode,
                         SocketCommunications *sock) {
  barrett::SafetyModule::PendantState ps;
  while (!(*done)) {
    (*mode) = sm->getMode();
    barrett::btsleep(0.01);
    sm->getPendantState(&ps);
    if (ps.hasFaults()) {
      std::cout << "Fault detected!" << std::endl;
      throw;
    }
    if ((*mode) == barrett::SafetyModule::ESTOP) {
      sock->sendData(kEStopString);
      std::cout << "Estopped" << std::endl;
      sock->closeSocket();
      throw;
    }
    barrett::btsleep(0.1);
  }
  std::cout << "Terminating monitorPendantState thread" << std::endl;
}

// Returns false if a quit command is received, true otherwise.
template <size_t DOF>
bool initializeJointEncoders(SocketCommunications *sock,
                             barrett::systems::Wam<DOF> *wam,
                             barrett::ProductManager *pm) {
  BARRETT_UNITS_TEMPLATE_TYPEDEFS(DOF);

  // Index pulses are 15 degrees away from the standard home position...
  const double kJencOffset = 15 * M_PI/180.0;
  // ... plus or minus 10 degrees.
  const double kJencTolerance = 10 * M_PI/180.0;
  // Currently JE's are only available for the first 4 DOF
  const size_t kNumJenc = 4;

  // Check for existence of joint encoders. Failure to find them indicates a
  // hardware problem.
  if (!wam->getLowLevelWam().hasJointEncoders()) {
    std::string msg = "ERROR: Joint encoders not found. Using motor encoders.";
    std::cout << msg << std::endl;
    sock->sendData(msg);
    return true;
  }
 
  // Define the positions at which we expect to find the index pulses for each
  // joint encoder.
  jp_type indexPositions(0.0);
  indexPositions[0] = 0.0 - kJencOffset;
  indexPositions[1] = -2.0 + kJencOffset;
  indexPositions[2] = 0.0 - kJencOffset;
  indexPositions[3] = M_PI - kJencOffset;

  jp_type jp = wam->getJointPositions();
  v_type dir = barrett::math::sign(indexPositions - jp);
  jp_type farSideOfIndexes = indexPositions + kJencTolerance * dir;

  // The wrist doesn't need to move.
  for (size_t i = kNumJenc; i < DOF; ++i) {
    farSideOfIndexes[i] = jp[i];
  }

  // For some systems, it is important that the WAM endpoint move straight up
  // before moving to the side to avoid damage to the tool. So use this
  // intermediate point for safety.
  jp_type safetyPoint = jp;
  safetyPoint[1] = farSideOfIndexes[1];
  safetyPoint[3] = farSideOfIndexes[3];

  // Set the number of times to do init sequence. If not already initialized,
  // move through the sequence once. Then check the values of ECMIN and ECMAX.
  // The first time the index pulse is found, these are usually set to large
  // values. If that is the case, they should be reset and done again.
  uint nMoves = 0;
  if (!jointEncodersInitialized(wam, kNumJenc)) {
    nMoves = 2;
  } else if (!checkEncoderCorrections(pm)) {
    nMoves = 1;
  }

  bool done = false;
  barrett::SafetyModule::SafetyMode mode;
  barrett::btsleep(0.1);
  boost::thread th(&monitorPendantState, pm->getSafetyModule(), &done, &mode, sock);

  // Only do the wait for user if movements are necessary.
  if (nMoves > 0) {
    std::string rec = "";
    std::string msg = "The WAM needs to initialize its joint encoders. Please "
      "make sure it has room, then press [e].";
    std::cout << msg << std::endl;
    sock->sendData(msg);
    do {
      if (mode == barrett::SafetyModule::IDLE) {
        done = true;
        th.join();
        return false;
      }
      rec.erase();
      if (sock->receiveData(&rec, false)) {
        std::cout << "Received message: " << rec << std::endl;
        if (rec.compare("q") == 0) {
          done = true;
          th.join();
          return false;
        }
      } else {
        barrett::btsleep(0.1);
      }
    } while (rec.compare(kWamZeroed) != 0);

    for (uint i = 0; i < nMoves; i++) {
      resetEncoderCorrections(pm);
      wam->moveTo(safetyPoint);
      wam->moveTo(farSideOfIndexes);
      wam->moveTo(safetyPoint);
      wam->moveHome();
    }
    wam->idle();
  }

  // If ECMIN and ECMAX are still not within reasonable bounds, this suggests a
  // hardware problem.
  if (!checkEncoderCorrections(pm)) {
    std::string msg =
      "ERROR: Joint encoder initialization failed. Using motor encoders.";
    std::cout << msg << std::endl;
    sock->sendData(msg);
    done = true;
    th.join();
    return true;
  }

  // Check difference between joint encoders and motor encoders. A large
  // difference would indicate miscalibration or a hardware problem.
  jp_type jp_from_jenc = wam->getLowLevelWam().getJointPositions(
      barrett::LowLevelWam<DOF>::PS_JOINT_ENCODER);
  jp_type jp_from_menc = wam->getLowLevelWam().getJointPositions(
      barrett::LowLevelWam<DOF>::PS_MOTOR_ENCODER);
  jp_type diff = jp_from_jenc - jp_from_menc;
  double diff_norm = 0;
  // only joints 1-4 have joint encoders
  for (size_t i = 0; i < 4; i++) {
    diff_norm += pow(fabs(diff[i]), 2);
  }
  diff_norm = pow(diff_norm, 0.5);
  if (diff_norm > 0.2) {
    std::string msg = "ERROR: Diff norm between joint and motor encoder is "
      " abnormally large. Using motor encoders.";
    std::cout << msg << std::endl;
    sock->sendData(msg);
  } else {
    wam->getLowLevelWam().setPositionSensor(
        barrett::LowLevelWam<DOF>::PS_JOINT_ENCODER);
    std::string msg = "Successfully activated joint encoders.";
    std::cout << msg << std::endl;
    sock->sendData(msg);
  }

  done = true;
  // Unclear why this sleep is necessary, but without it there is an exception
  // from puck comms. Maybe due to not enough time between this pendant thread
  // ending and the one in the main program starting?
  barrett::btsleep(0.5);
  th.join();
  if (mode == barrett::SafetyModule::IDLE) {
    return false;
  } else {
    return true;
  }
}

#endif  // INITIALIZE_JOINT_ENCODERS_H_
