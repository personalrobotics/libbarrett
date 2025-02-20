/*
 * ex08_teach_and_play.cpp
 *
 *  Created on: Sep 29, 2009
 *      Author: dc
 */

#include <iostream>
#include <vector>
#include <string>

#include <boost/ref.hpp>
#include <boost/bind/bind.hpp>
using namespace boost::placeholders;
#include <boost/tuple/tuple.hpp>

#define EIGEN_USE_NEW_STDVECTOR
#include <Eigen/StdVector>

#include <barrett/detail/stl_utils.h>  // waitForEnter()
#include <barrett/math.h>
#include <barrett/units.h>
#include <barrett/systems.h>
#include <barrett/log.h>
#include <barrett/products/product_manager.h>

#include <barrett/standard_main_function.h>

#include <nlohmann/json.hpp>


using namespace barrett;
using detail::waitForEnter;
using systems::connect;
using systems::disconnect;
using systems::reconnect;
using json = nlohmann::json;

template<size_t DOF>
int wam_main(int argc, char** argv, ProductManager& pm, systems::Wam<DOF>& wam) {
	BARRETT_UNITS_TEMPLATE_TYPEDEFS(DOF);
	typedef boost::tuple<double, jp_type> jp_sample_type;

	wam.gravityCompensate();

	systems::Ramp time(pm.getExecutionManager());

	std::ifstream f("./trajectory.json");
 	if (!f.is_open()) {
    	std::cout << "Unable to located file." << std::endl;
		wam.idle();
  	}
  	json json_data = json::parse(f);
  	std::string debug = "Loaded: " + json_data.dump(4);
	std::cout << debug << std::endl;

    auto init_data = json_data["q_init"];
	jp_type arm_init_joint_positions;  // = data;
	for (size_t i = 0; i < arm_init_joint_positions.size(); i++) {
		arm_init_joint_positions[i] = init_data[i];
	}

	std::cout << "Press [Enter] to move to the init position." << std::endl;
	waitForEnter();

    wam.moveTo(arm_init_joint_positions, true, 0.2, 0.2);

	std::cout << "Done moving to initial pose" << std::endl;

    auto data = json_data["positions"];
	std::vector<jp_sample_type, Eigen::aligned_allocator<jp_sample_type> > vec;

	double current_time = 0;
	for (auto& p: data)
	{
		jp_type arm_position; 
		for (size_t i = 0; i < arm_position.size(); i++) {
			arm_position[i] = p[i];
		}
		vec.push_back(boost::make_tuple(current_time, arm_position));
		current_time += 0.002;
	}

	std::cout << "Press [Enter] to move to start following the trajectory." << std::endl;
	waitForEnter();
	 
	math::Spline<jp_type> spline(vec);

	// First, move to the starting position
	wam.moveTo(spline.eval(spline.initialS()));

	// Then play back the recorded motion
	time.stop();
	time.setOutput(spline.initialS());

	systems::Callback<double, jp_type> trajectory(boost::ref(spline));
	connect(time.output, trajectory.input);
	wam.trackReferenceSignal(trajectory.output);

	time.start();

	while (trajectory.input.getValue() < spline.finalS()) {
		usleep(100000);
	}

	std::cout << "Press [Enter] to idle the WAM." << std::endl;
	waitForEnter();
	wam.idle();

	pm.getSafetyModule()->waitForMode(SafetyModule::IDLE);

	return 0;
}
