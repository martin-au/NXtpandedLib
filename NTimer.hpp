/*
 * NTimer.hpp
 *
 *  Created on: 01.12.2013
 *      Author: Martin
 */

#ifndef __NTIMER_HPP_
#define __NTIMER_HPP_

#include "C:/cygwin/nxtOSEK/ecrobot/c++/device/Clock.h"

class NTimer {
private:
	ecrobot::Clock clk;
	U32 time;
	U32 tpause;
	bool started;
public:
	NTimer();
	~NTimer() {}

	void start(U32 offset = 0);
	U32 stop();
	void reset();
	U32 getLast() const {
		return time;
	}
	U32 getPause() const {
		if(!started && tpause > 0) {
			// timer started + stopped
			return tpause - 1 + clk.now() - time;
		}
		return tpause;
	}

	U32 now() const {
		return clk.now();
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


NTimer::NTimer() : clk(), time(0), tpause(0), started(false) {}



void NTimer::start(U32 offset) {
	if(started) return;

	time += offset;
	started = true;

	if(tpause > 0) {
		--tpause;
		tpause += clk.now() - time;
	} else {
		clk.reset();
	}
}


U32 NTimer::stop() {
	if(!started) return time;

	time = clk.now() - tpause;

	++tpause;
	started = false;
	return time;
}


// call stop before reset!!!!!
void NTimer::reset() {
	time = 0;
	tpause = 0;
	started = false;
}



#endif /* NTIMER_HPP_ */
