/*
 * Motorcontroller.hpp
 *
 *  Created on: 28.01.2014
 *      Author: Martin
 */

#ifndef __MOTORCONTROLLER_HPP_
#define __MOTORCONTROLLER_HPP_

// #define NXPL_MOTORCONTROLLER_EVENTS_ON
#define NXPL_MOTORCONTROLLER_STALL_RESET_ON

/** \file
 *	\ingroup NxtIO
*/

#include "Mutex.hpp"
#include "Uncopyable.hpp"

#include "../../ecrobot/c++/device/Motor.h"

// Sleep
extern "C" {
	#include "../../ecrobot/c/rtoscalls.h"
}


namespace nxpl {

/** \brief Motorcontroller for NXT motors position controlling.
 *
 * This is a aggressive motorcontroller. It tries to move the motor as fast as possible to the setpoint and
 * then hold the position consequently.
 * Its important to call the Motorcontroller::process() method in a rate monotonic task.
 * After that you can use all functions.
 */
class Motorcontroller {
private:
	ecrobot::Motor* mot;
	mutex_t &controlMtx;

	//long M_DELAY;
	S16  	 M_SCALE;

	S16      amax;
	S16      vmax;

	S16      phigh;
	//S8       mmot;
	bool     mon;
	bool     mgo;
	bool     mup;
#ifdef NXPL_MOTORCONTROLLER_STALL_RESET_ON
	bool     mreset;
#endif
	S32      mtx;
	S32      mx;
	S32      mv;
	S32      ma;
	S32      mp;
	S32      me;

    #ifdef NXPL_MOTORCONTROLLER_EVENTS_ON
	bool waitForEvent;
	TaskType idOfWaitingForMoveDoneEventTask;
	const EventMaskType &moveDoneEvent;
	#endif

	void stopAtCurrentPos(S16 pwr, bool waitStop) {
		S32 posNow = getRelPos();
		moveAbs(posNow, pwr, waitStop);
	}

public:

#ifndef NXPL_MOTORCONTROLLER_EVENTS_ON
	/**
	 * \brief Construct and initialize the motorcontroller control parameters.
	 *
	 * Set control parameters for given motor.
	 * Amax represents the maximum Acceleration that will be achieved
	 * when the motor is stationary and the maximum power is applied.
	 * Its mainly for holding the motor in position. High amax-> high overshooting.
	 * Vmax is the maximum Velocity that will be achieved.
	 * High vmax -> smoother breaking, no overshooting.
	 * Same mutex should be used on all motors.
	 *
	 * @param motor The motor which should be controlled.
	 * @param controlMutex Mutex for controlling all Motors
	 * @param paraAmax Control-parameter maximum Acceleration.
	 * @param paraVmax Control-parameter maximum Velocity.
	*/
	Motorcontroller(ecrobot::Motor *motor, mutex_t &controlMutex, S16 paraAmax, S16 paraVmax, S16 initialMotorPwr = 50)
		: mot(motor),
		  controlMtx(controlMutex),
		  M_SCALE(12),
		  amax(paraAmax),
		  vmax(paraVmax),
		  phigh(initialMotorPwr),
		  mon(0),
		  mgo(0),
		  mup(0),
#ifdef NXPL_MOTORCONTROLLER_STALL_RESET_ON
		  mreset(0),
#endif
		  mtx(0),
		  mx(0),
		  mv(0),
		  ma(0),
		  mp(0),
		  me(0) {
		mot->setBrake(true);
	}

#else
	/**
	 * \brief Construct and initialize the motorcontroller control parameters.
	 *
	 * Set control parameters for given motor.
	 * Amax represents the maximum Acceleration that will be achieved
	 * when the motor is stationary and the maximum power is applied.
	 * Its mainly for holding the motor in position. High amax-> high overshooting.
	 * Vmax is the maximum Velocity that will be achieved.
	 * High vmax -> smoother breaking, no overshooting.
	 * Same mutex should be used on all motors.
	 *
	 * @param motor The motor which should be controlled.
	 * @param controlMutex Mutex for controlling all Motors
	 * @param userMoveDoneEvent this event will be set if moveIsDone, see Motorcontroller::waitMoveEvent()
	 * @param paraAmax Control-parameter maximum Acceleration.
	 * @param paraVmax Control-parameter maximum Velocity.
	*/
	Motorcontroller(ecrobot::Motor *motor, mutex_t &controlMutex, const EventMaskType &userMoveDoneEvent,S16 paraAmax, S16 paraVmax, S16 initialMotorPwr = 50)
		: mot(motor),
		  controlMtx(controlMutex),
		  M_SCALE(12),
		  amax(paraAmax),
		  vmax(paraVmax),
		  phigh(initialMotorPwr),
		  mon(0),
		  mgo(0),
		  mup(0),
		  mreset(0),
		  mtx(0),
		  mx(0),
		  mv(0),
		  ma(0),
		  mp(0),
		  me(0),
		  waitForEvent(false),
		  moveDoneEvent(userMoveDoneEvent) {
		mot->setBrake(true);
	}
#endif

	/**
	 * \brief You should use controllerOff() and/or stopAtCurrentPosition before destructing!
	 */
	~Motorcontroller() {
		// this->controllerOff();
	}

	/**
	  \brief Set control parameter for one motor.

	 * Amax represents the maximum Acceleration that will be achieved
	 * when the motor is stationary and the maximum power is applied.
	 * Its mainly for holding the motor in position. High amax-> high overshooting.
	 *
	 * \param paraAmax Control-parameter maximum Acceleration.
	*/
	void setAmax(S16 paraAmax) {
		amax = paraAmax;
	}

	/**
	 * \brief Set control parameter for one motor.
	 * Vmax is the maximum Velocity that will be achieved.
	 * High vmax -> smoother breaking, no overshooting.
	 *
	 * \param paraVmax Control-parameter maximum Velocity.
	 */
	void setVmax(S16 paraVmax) {
		vmax = paraVmax;
	}

	/**
	 * \brief Turns the controller on.
	 *
	 * Note that Motorcontroller will be started automatically when calling a move function.
     * Its mainly for saving process time with controllerOff/On.
	 */
	void controllerOn() {
		controlMtx.acquire();
		mon = true;
		controlMtx.release();
	}

	/**
	 * \brief Turns the controller off.
	 *
	 * Waits until motor is on setpoint then brake the motor.
	 * Controller intern regulation parameters are reseted.
	 * Motor rotation count is not reseted!
	 */
	void controllerOff();

	/** \brief Checks if motor is on setpoint.
	 *
	 * @return true if motor is on setpoint.
	 */
	bool moveDone() const {
	   bool go;
	   controlMtx.acquire();
	   go = mgo;
	   controlMtx.release();
	   return !go;
	}

	/** \brief Wait until motor is on setpoint.
	*
	* This function is a blocking function. This means it will return when the motor is on setpoint.
	* Sleep is used for waiting: see ecrobot API what you have to implement in oil file!
	*/
	void waitMove() const {
	   do {
	     Sleep(1);
	   } while (!moveDone());
	}

#ifdef NXPL_MOTORCONTROLLER_EVENTS_ON
	void waitMoveEvent() {
		GetTaskID(&idOfWaitingForMoveDoneEventTask);
		controlMtx.acquire();
		waitForEvent = true;
		controlMtx.release();
		WaitEvent(moveDoneEvent);
		ClearEvent(moveDoneEvent);
	}
#endif

	/** \brief Move motor relative.
	 *
	 * Move relative means move motor to position: actual position + angle.
	 * If you pass pwr 0 it will take the last used motorpower.
	 *
	 * @param angle Angle in degrees.
	 * @param pwr   The maximum allowed motor PWM power. (0 - 100)
	 * @param waitMove If true: Wait until move complete. (blocking)
	 * @param off
	 */
	void moveRel(S32 angle, S16 pwr = 0, bool waitMove = true, S32 off = 0);

	/** \brief Move motor absolute.
	 *
	 * Move absolute means move motor to give position from absolute 0° position.
	 * If you pass pwr 0 it will take the last used motorpower.
	 *
	 * @param pos  Angle in degrees.
	 * @param pwr   The maximum allowed motor PWM power. (0 - 100)
	 * @param waitMove If true: Wait until move complete. (blocking)
	 */
	void moveAbs(S32 pos, S16 pwr = 0, bool waitMove = true);

	/** \brief Move motor.
	 *
	 * Move motor with given power. This will turn the controller off.
	 * The direction is regulated with the sign of pwr.
	 *
	 * @param pwr  The maximum allowed motor PWM power. (-100 to 100)
	 */
	void move(S16 pwr) {
		this->controllerOff();
		mot->setPWM(pwr);
	}

	/** \brief Stop motor.
	 *
	 * Stop motor and set in breaking or floating mode. This will turn the controller off.
	 *
	 * @param brake If true set brake else set floating mode.
	 */
	void off(bool brake) {
		this->controllerOff();
		mot->setBrake(brake);
		mot->setPWM(0);
	}

	/** \brief Stops the Motor softly at current position
	 *
	 * Cancels the actual move to any setpoint by stopping
	 * the motor softly at the actual position.
	 * Use moveDone()/waitMove()/waitMoveEvent() to see if motor is stopped.
	 *
	 * @param waitStop Wait until motor is stopped (blocking)
	 */
	void stopAtCurrentPosition(bool waitStop) {
		stopAtCurrentPos(30, waitStop);
	}

	/** \brief Emergency-Stops the Motor at current position
	 *
	 * Cancels the actual move to any setpoint by stopping
	 * the motor hardly/aggressive at the actual position.
	 * Use moveDone() to see if motor is stopped or use waitStop for blocking function.
	 * You may use this function in the task where process is running to check for out of range!
	 *
	 * @param waitStop Wait until motor is stopped (blocking)
	 */
	void emergencyStop(bool waitStop) {
		stopAtCurrentPos(87, waitStop);
	}

	/** \brief Get motor absolute position
	 *
	 * The absolute position for the motor is between 0° - 359°
	 *
	 * @return absolute position
	 */
	S32 getAbsPos() const;

	/** \brief Get motor relative position
	 *
	 * The relative position is the angle count from 0-Position to actual position.
	 *
	 * @return relativ position
	 */
	S32 getRelPos() const {
		return mot->getCount();
	}

	/** \brief Reset position(counter)
	 *
	 *  This will reset the position of the motor.
	 *  The actual position is now the new zero-position.
	 *  Use this function to give your motor initial position or
	 *  if you moved the motor after controllerOff(), otherwise motorcontroller
	 *  will try to reach last known set-point!
	 */
	void resetPos() {
		controllerOff();
		mot->resetCount();
	}

#ifdef NXPL_MOTORCONTROLLER_STALL_RESET_ON
	/**
	 * \brief Reset the motor position.
	 *
	 * Reset the motor by turning the shaft until stalling.
	 * Be aware of using this function. The position is used as absolute 0 position.
	 * This function use process(). <br>
	 * Check with moveDone()/waitMove()/waitMoveEvent() if motor is reseted.
	 *
	 * @param pwr Output power, from -50 to +50, use the sign for direction.
	 * @param if true this functions is blocking until move is done.
	 *
	*/
	void resetMotorPos(S16 pwr, bool waitReset);
#endif

	/** \brief Control-Algorithm process.
	 *
	 * Call this function in a rate monotonic scheduled task.
	 * Cycle times around 10 ms are ok. Priority should be very high!
	 * All the control work is done in this function.
	 */
	void process();
};


} /* namespace nxpl */


#endif /* __MOTORCONTROLLER_HPP_ */
