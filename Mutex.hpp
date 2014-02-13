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
#include "RefCount.hpp"

struct mutex_t {
private:
	ResourceType mtx;
public:
	mutex_t(ResourceType resource) {
		mtx = resource;
	}
	StatusType acquire() {
		return GetResource(mtx);
	}
	StatusType release() {
		return ReleaseResource(mtx);
	}
};


class LockGuard: private Uncopyable {
private:
	mutex_t &_ref;
public:
	LockGuard(mutex_t &mutex) : _ref(mutex) {
		_ref.acquire();
	}

	~LockGuard() {
		_ref.release();
	}

	// use this to Acquire/Release manually
	mutex_t &get() const {return _ref;}
};


class SharedLockGuard {
private:
	mutex_t &_ref;
	RefCount *refcnt;
public:
	SharedLockGuard(mutex_t &mutex) : _ref(mutex), refcnt(new RefCount) {
		_ref.acquire();
		refcnt->add();
	}

	SharedLockGuard(const SharedLockGuard &slg) : _ref(slg._ref), refcnt(slg.refcnt) {
		// Copy constructor
		// Copy the data and reference pointer
		// and increment the reference count
		refcnt->add();
	}

	~SharedLockGuard() {
		if (refcnt->release() == 0) {
			_ref.release();
			delete refcnt;
		}
	}

	SharedLockGuard& operator=(const SharedLockGuard& slg) {
        if (this != &slg) {
            if(refcnt->release() == 0) {
            	_ref.release();
                delete refcnt;
            }

            _ref = slg._ref;
            refcnt = slg.refcnt;
            refcnt->add();
        }
        return *this;
    }

	// use this to Acquire/Release manually
	mutex_t &get() const {return _ref;}
};


#endif /* LOCKGUARD_HPP_ */
