/*
 * LockGuard.hpp
 *
 *  Created on: 08.12.2013
 *      Author: Martin
 */

#ifndef __MUTEX_HPP_
#define __MUTEX_HPP_

/** \file
 *	\ingroup System
*/

extern "C" {
	#include "../../toppers_osek/include/kernel.h"
}

#include "..\..\ecrobot\c\ecrobot_types.h"

#include "Uncopyable.hpp"
#include "RefCount.hpp"

namespace nxpl {

/**
 * \brief Resource wrapper implements simple, modern and advanced mutex functions.
 */
struct NMutex {
private:
	ResourceType mtx;
public:
	/**
	 * \brief Create mutex from OSEK-Resource
	 *
	 * Create a resource in the OIL-File and pass it to mutex_t.
	 *
	 * \sa http://www.cs.ru.nl/lab/nxt/nxtosek/OSEK/OSEK_API.html#0.1_58
	 *
	 * @param resource Reference to resource
	 */
	NMutex(ResourceType resource)
			: mtx(resource)
	{}

	/**
	 * \brief Acquire the lock to the mutex.
	 *
	 * This call serves to enter critical sections in the code that are
     * assigned to the resource referenced by mutex_t. A critical
	 * section shall always be left using mutex_t::release().
	 *
	 * \sa http://www.cs.ru.nl/lab/nxt/nxtosek/OSEK/OSEK_API.html#0.1_58
	 *
	 * @return No error, E_OK
	 */
	StatusType acquire() {
		return GetResource(mtx);
	}

	/**
	 * \brief Release the lock on the mutex.
	 *
	 * Release is the counterpart of mutex_t::acquire() and
     * serves to leave critical sections in the code that are assigned to
     * the resource referenced by mutex_t.
	 *
	 * \sa http://www.cs.ru.nl/lab/nxt/nxtosek/OSEK/OSEK_API.html#0.1_59
	 *
	 * @return No error, E_OK
	 */
	StatusType release() {
		return ReleaseResource(mtx);
	}
};

/**
 * \brief A lock guard is an object that manages a mutex object by keeping it always locked.
 *
 * On construction, the mutex object is locked, and on destruction,
 * the mutex is unlocked. It is the simplest lock,
 * and is specially useful as an object with automatic duration that lasts until the end of its context.
 * In this way, it guarantees the mutex object is always properly unlocked at the end of the object lifetime.
 *
 * Do not use this in task blocks (TASK() {}) Always release before TerminateTask, ChainTask ...
 */
class LockGuard: private Uncopyable {
private:
	NMutex &_ref;
public:
	/** \brief Constructs a LockGuard object that keeps mutex locked.
	 *
	 * @param mutex
	 */
	LockGuard(NMutex &mutex) : _ref(mutex) {
		_ref.acquire();
	}

	~LockGuard() {
		_ref.release();
	}

	/** \brief Get the mutex. managed by LockGuard.
	 *
	 * @return Mutex object.
	 */
	NMutex &get() const {return _ref;}
};

/**
 * \brief A lock guard is an object that manages a mutex object by keeping it always locked.
 *
 * SharedLockGuard is the same as LockGuard but its allowed to copy SharedLockGuard.
 * On construction, the mutex object is locked, and when the last object with a reference to the mutex is destroyed,
 * the mutex is unlocked. Its used for passing it to functions/containers.
 *
 * Do not use this in task blocks (TASK() {}) Always release before TerminateTask, ChainTask ...
 */
class SharedLockGuard {
private:
	NMutex &_ref;
	RefCount *refcnt;
public:
	/** \brief Constructs a SharedLockGuard object that keeps mutex locked.
	 *
	 * @param mutex
	 */
	SharedLockGuard(NMutex &mutex) : _ref(mutex), refcnt(new RefCount) {
		_ref.acquire();
		refcnt->add();
	}

	/** \brief Copyconstructor
	 *
	 * @param slg
	 */
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

	/** \brief Get the mutex. managed by SharedLockGuard.
	*
	* @return Mutex object.
	*/
	NMutex &get() const {return _ref;}
};

}

#endif /* LOCKGUARD_HPP_ */
