/**
 *	Copyright 2009-2014 Barrett Technology <support@barrett.com>
 *
 *	This file is part of libbarrett.
 *
 *	This version of libbarrett is free software: you can redistribute it
 *	and/or modify it under the terms of the GNU General Public License as
 *	published by the Free Software Foundation, either version 3 of the
 *	License, or (at your option) any later version.
 *
 *	This version of libbarrett is distributed in the hope that it will be
 *	useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License along
 *	with this version of libbarrett.  If not, see
 *	<http://www.gnu.org/licenses/>.
 *
 *
 *	Barrett Technology Inc.
 *	73 Chapel Street
 *	Newton, MA 02458
 */

/**
 * @file os.cpp
 * @date 03/28/2012
 * @author Dan Cody
 * 
 */


#include <stdexcept>
#include <iostream>
#include <cassert>

#include <syslog.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/timerfd.h>

#ifdef BARRETT_XENOMAI
#include <native/task.h>
#include <native/timer.h>
#endif

#include <boost/thread.hpp>
#include <boost/date_time.hpp>

#include <barrett/detail/stacktrace.h>
#include <barrett/detail/os.h>
#include <barrett/os.h>


#ifdef BARRETT_XENOMAI
// Xenomai helper function
inline RTIME secondsToRTIME(double s) {
	return static_cast<RTIME>(s * 1e9);
}

// Xenomai requires at least one call to mlockall() per process. Also, a signal
// handler is installed to trap transitions from primary execution mode to
// secondary execution mode; this aids in identifying code that breaks Xenomai's
// realtime guarantee.
namespace {  // Using an anonymous namespace because no other code needs to
			 // interact with these declarations. It is necessary to construct a
			 // single instance of InitXenomai.
	extern "C" {
	void warnOnSwitchToSecondaryMode(int)
	{
		barrett::logMessage("WARNING: Switched out of RealTime. Stack-trace:", true);
		barrett::detail::syslog_stacktrace();
	}
	}

	class InitXenomai {
	public:
		InitXenomai() {
			// Avoids memory swapping for this program
			mlockall(MCL_CURRENT|MCL_FUTURE);

			// Handler for warnings about falling out of primary mode
			signal(SIGXCPU, &warnOnSwitchToSecondaryMode);
		}
	};
	// Static variables are initialized when the module is loaded. This causes the
	// InitXenomai::InitXenomai() ctor to be called at module load time.
	static InitXenomai ignore;
}
#endif


namespace barrett {


void btsleep(double duration_s)
{
	// Why do we need duration_s to be > 1 us? 
	// This would call abort() for durations under 1 us, which seems bad!
	//assert(duration_s > 1e-6);  // Minimum duration is 1 us
	
	boost::this_thread::sleep(boost::posix_time::microseconds(long(duration_s * 1e6)));
}

void btsleepRT(double duration_s)
{
#ifdef BARRETT_XENOMAI
	assert(duration_s > 1e-9);  // Minimum duration is 1 ns
	int ret = rt_task_sleep(RTIME(duration_s * 1e9));
	if (ret != 0) {
		(logMessage("%s: rt_task_sleep() returned error %d.") % __func__ % ret).raise<std::runtime_error>();
	}
#else
	btsleep(duration_s);
#endif
}

void btsleep(double duration_s, bool realtime)
{
	if (realtime) {
		btsleepRT(duration_s);
	} else {
		btsleep(duration_s);
	}
}

#ifndef BARRETT_XENOMAI
// Record the time program execution began
const boost::posix_time::ptime START_OF_PROGRAM_TIME = boost::posix_time::microsec_clock::local_time();
#endif
double highResolutionSystemTime()
{
#ifdef BARRETT_XENOMAI
	return 1e-9 * rt_timer_read();
#else
	// total_nanoseconds() returns a long, which is insufficient beyond 2 seconds on a 32-bit system.
	// Plus, the subtraction is performed in microseconds, so there is no advantage to converting to nano.
	//return (boost::posix_time::microsec_clock::local_time() - START_OF_PROGRAM_TIME).total_nanoseconds() * 1e-9;
	return (boost::posix_time::microsec_clock::local_time() - START_OF_PROGRAM_TIME).total_microseconds() * 1e-6;
#endif
}

// period in us
static int make_periodic (unsigned int period, struct periodic_info *info)
{
	int ret;
	unsigned int ns;
	unsigned int sec;
	int fd;
	struct itimerspec itval;

	/* Create the timer */
	fd = timerfd_create (CLOCK_MONOTONIC, 0);
	info->wakeups_missed = 0;
	info->timer_fd = fd;
	if (fd == -1)
		return fd;

	/* Make the timer periodic */
	sec = period/1000000;
	ns = (period - (sec * 1000000)) * 1000;
	itval.it_interval.tv_sec = sec;
	itval.it_interval.tv_nsec = ns;
	itval.it_value.tv_sec = sec;
	itval.it_value.tv_nsec = ns;
	ret = timerfd_settime (fd, 0, &itval, NULL);
	return ret;
}

static void wait_period (struct periodic_info *info)
{
	unsigned long long missed;
	int ret;

	/* Wait for the next timer event. If we have missed any the
	   number is written to "missed" */
	ret = read (info->timer_fd, &missed, sizeof (missed));
	if (ret == -1)
	{
		perror ("read timer");
		return;
	}

	/* "missed" should always be >= 1, but just to be sure, check it is not 0 anyway */
	if (missed > 0)
		info->wakeups_missed = (missed - 1);
}

PeriodicLoopTimer::PeriodicLoopTimer(double period_, int threadPriority) :
		firstRun(true), period(period_), releasePoint(-1.0)
{
#ifdef BARRETT_XENOMAI
	int ret;

	// Try to become a Xenomai task
	ret = rt_task_shadow(NULL, NULL, threadPriority, 0);
	// EBUSY indicates the current thread is already a Xenomai task
	if (ret != 0  &&  ret != -EBUSY) {
		(logMessage("PeriodicLoopTimer::%s: rt_task_shadow(): (%d) %s")
				% __func__ % -ret % strerror(-ret)).raise<std::runtime_error>();
	}

	ret = rt_task_set_periodic(NULL, TM_NOW, secondsToRTIME(period));
	if (ret != 0) {
		(logMessage("PeriodicLoopTimer::%s: rt_task_set_periodic(): (%d) %s")
				% __func__ % -ret % strerror(-ret)).raise<std::runtime_error>();
	}
#else
	logMessage("PeriodicLoopTimer is using timer_fd");
	make_periodic (period * 1e6, &info);
#endif
}

unsigned long PeriodicLoopTimer::wait()
{
#ifdef BARRETT_XENOMAI
	unsigned long missedReleasePoints;

	int ret = rt_task_wait_period(&missedReleasePoints);
	if (ret != 0  &&  ret != -ETIMEDOUT) {  // ETIMEDOUT means that we missed a release point
		(logMessage("%s: rt_task_wait_period(): (%d) %s") % __func__ % -ret % strerror(-ret)).raise<std::runtime_error>();
	}

	return missedReleasePoints;
#else
	wait_period(&info);
	return info.wakeups_missed;
/*
	const double now = highResolutionSystemTime();	// Get the current time
	const double remainder = releasePoint - now; 	// Calculate the amount of time remaining until our next release point
	if (remainder <= 0) { 							// If we are already past our next scheduled release point, then we missed it
		releasePoint = now + period; 				// Schedule a new release point at period seconds into the future

		if (firstRun) {
			firstRun = false;
			return 0;								// Don't worry if we missed the very first release point
		} else {									// Otherwise, report the number of periods missed
			return ceil(-remainder / period);		// ceil() returns the smallest integer not less than arg
		}
	} else {
		// Calculate the new releasePoint based on the old one.
		// This eliminates drift (on average) due to over/under sleeping.
		releasePoint += period;
		btsleep(remainder);
		return 0;
	}
*/
#endif
}



detail::LogFormatter logMessage(const std::string& message, bool outputToStderr)
{
	return detail::LogFormatter(message, outputToStderr);
}


namespace detail {

void LogFormatter::print()
{
	// Make sure we only print once
	if (printed) {
		return;
	}
	printed = true;

	std::string message = str();
	if (ose) {
		std::cerr << message;
		// The message should always have one newline
		if (message[message.size() - 1] != '\n') {
			std::cerr << std::endl;
		}
	}
	// Use a trivial format string in case message contains '%'
	syslog(LOG_ERR, "%s", message.c_str());
}

}
}
