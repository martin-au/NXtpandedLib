/*
 * LockGuard.hpp
 *
 *  Created on: 08.12.2013
 *      Author: Martin
 */

#ifndef __MUTEX_HPP_
#define __MUTEX_HPP_


extern "C" {
	#include "C:/cygwin/nxtOSEK/toppers_osek/include/kernel.h"
	#include "kernel_id.h"
	#include "C:/cygwin/nxtOSEK/ecrobot/c/ecrobot_interface.h"
}

#include "C:/cygwin/nxtOSEK/lejos_nxj/src/nxtvm/platform/nxt/mytypes.h"
#include "Uncopyable.hpp"

struct mutex_t {
private:
	ResourceType mtx;
public:
	mutex_t(ResourceType resource) {
		mtx = resource;
	}
	StatusType Acquire() {
		return GetResource(mtx);
	}
	StatusType Release() {
		return ReleaseResource(mtx);
	}
};


class LockGuard: private Uncopyable {
public:

	LockGuard(mutex_t &mutex) : _ref(mutex) {
		_ref.Acquire();
	}

	~LockGuard() {
		_ref.Release();
	}

private:
	mutex_t &_ref;
};


#endif /* LOCKGUARD_HPP_ */
