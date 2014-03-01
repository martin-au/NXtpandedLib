/*
 * NTimer.hpp
 *
 *  Created on: 01.12.2013
 *      Author: Martin
 */

#ifndef __NTIMER_HPP_
#define __NTIMER_HPP_

#include "C:/cygwin/nxtOSEK/ecrobot/c++/device/Clock.h"

namespace nxpl {

/** \brief A advanced timer for measurements.
 */
class NTimer {
private:
	ecrobot::Clock clk;
	U32 time;
	U32 tpause;
	bool started;

	// calling this functions makes only sense when tpause > 0 (&&!started)
	U32 stopTimeStamp() const {
		return time + tpause - 1;
	}
	// calling this functions makes only sense when tpause > 0 (&&!started)
	U32 timeSinceStopped() const {
		return clk.now() - stopTimeStamp();
	}

public:
	/**
	 * \brief Constructor.
	 * Resets the timer to 0.
	 */
	NTimer() : clk(), time(0), tpause(0), started(false) {}
	~NTimer() {}

	/** \brief Start timer.
	 */
	void start();

	/** \brief Stop timer.
	 *
	 * @return Relative time since first call of NTimer::start excluding all pauses.
	 */
	U32 stop();

	/** \brief Reset timer to time 0.
	 * After that the timer waits for next NTimer::start()
	 */
	void reset();

	/** \brief Get last measured time.
	 * A call to this function only makes sense after first NTimer::stop()
	 * @return Relative time since first call of NTimer::start excluding all pauses.
	 */
	U32 getLast() const {
		return time;
	}

	/** \brief Get the sum of all pauses since first call to NTimer::start()
	 *
	 * @return Sum of all pauses.
	 */
	U32 getPause() const {
		if(!started && tpause > 0) {
			return timeSinceStopped() + tpause - 1;
		}
		return tpause;
	}

	/**
	 * \brief Get the actual relative time since first NTimer::start() including all pauses.
	 * @return Actual time.
	 */
	U32 now() const {
		return clk.now();
	}

	/**
	 * \brief Get the actual relative time since first NTimer::start() excluding all pauses.
	 * @return Actual time excluding all pauses.
	 */
	U32 nowNoPause() const {
		// Timer in running mode
		if(started) {
			return clk.now() - tpause;
		// Timer stopped
		} else if(tpause > 0) {
			return clk.now() - tpause - 1;
		}
		// Timer reseted
		return 0;
	}

	/** \brief Wait time.
	 *
	 * If pause = true the waitTime will be calculated to pause.
	 * Its better to use sleep in multithreaded environments because it lets the other tasks time to execute.
	 *
	 * @param waitTime Time to wait.
	 * @param pause If true treat waiting as pause.
	 */
	void wait(U32 waitTime, bool pause = false) {
		clk.wait(waitTime);
		if(pause && started) {
			tpause += waitTime;
		}
	}

	/** \brief Sleep task.
	 *
	 * If pause = true the sleepTime will be calculated to pause.
	 * Sleep takes the calling task to sleeping mode which allows other task to execute.
	 *
	 * @param sleepTime Time to sleep.
	 * @param pause If true treat sleeping as pause.
	 */
	void sleep(U32 sleepTime, bool pause = false) {
		clk.sleep(sleepTime);
		if(pause && started) {
			tpause += sleepTime;
		}
	}
};



void NTimer::start() {
	if(started) return;

	// user called start then stop and then start again
	if(tpause > 0) {
		// ! important remove offset -> see stop() for detail why
		--tpause;
		tpause += timeSinceStopped();
	} else { // start from zero
		clk.reset();
	}
	started = true;
}


U32 NTimer::stop() {
	if(!started) return time;

	time = clk.now() - tpause;

	// we need this offset so that the next call of start knows that the
	// timer is in stop mode and not reseted! We save one variable..
	++tpause;
	started = false;
	return time;
}


void NTimer::reset() {
	time = 0;
	tpause = 0;
	started = false;
}

}

#endif /* __NTIMER_HPP_ */
