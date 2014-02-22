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
	NTimer() : clk(), time(0), tpause(0), started(false) {}
	~NTimer() {}

	void start();
	U32 stop();
	void reset();
	U32 getLast() const {
		return time;
	}
	U32 getPause() const {
		if(!started && tpause > 0) {
			return timeSinceStopped() + tpause - 1;
		}
		return tpause;
	}

	U32 now() const {
		return clk.now();
	}

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

	void wait(U32 waitTime, bool pause = false) {
		clk.wait(waitTime);
		if(pause && started) {
			tpause += waitTime;
		}
	}

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
