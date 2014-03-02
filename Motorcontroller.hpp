/*
 * Motorcontroller.hpp
 *
 *  Created on: 28.01.2014
 *      Author: Martin
 */

#ifndef __MOTORCONTROLLER_HPP_
#define __MOTORCONTROLLER_HPP_

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
	//static const S32 nxtMotorPorts = NUM_PORT_M;
	ecrobot::Motor* mot;

	//long M_DELAY;
	S16  	 M_SCALE;

	S16      amax;
	S16      vmax;

	S16      phigh;
	//S8       mmot;
	bool     mon;
	bool     mgo;
	bool     mup;
	S32      mtx;
	S32      mx;
	S32      mv;
	S32      ma;
	S32      mp;
	S32      me;

public:

	/**
	 * \brief Construct and initialize the motorcontroller control parameters.
	 *
	 * Set control parameters for given motor.
	 * Amax represents the maximum Acceleration that will be achieved
	 * when the motor is stationary and the maximum power is applied.
	 * Its mainly for holding the motor in position. High amax-> high overshooting.
	 * Vmax is the maximum Velocity that will be achieved.
	 * High vmax -> smoother breaking, no overshooting.
	 *
	 * @param motor The motor which should be controlled.
	 * @param paraAmax Control-parameter maximum Acceleration.
	 * @param paraVmax Control-parameter maximum Velocity.
	*/
	Motorcontroller(ecrobot::Motor *motor, S16 paraAmax, S16 paraVmax)
		: mot(motor),
		  M_SCALE(12),
		  amax(paraAmax),
		  vmax(paraVmax),
		  phigh(87),
		  //mmot(0),
		  mon(0),
		  mgo(0),
		  mup(0),
		  mtx(0),
		  mx(0),
		  mv(0),
		  ma(0),
		  mp(0),
		  me(0) {
		mot->setBrake(true);
	}

	/**
	 * \brief Destructor calls Motorcontroller::controllerOff();
	 */
	~Motorcontroller() {
		// TODO Problem because we use Sleep
		this->controllerOff();
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
		// GetResource
		amax = paraAmax;
		// ReleaseResource
	}

	/**
	 \brief Set control parameter for one motor.

	 * Vmax represents the maximum Acceleration that will be achieved
	 * when the motor is stationary and the maximum power is applied.
	 * Its mainly for holding the motor in position. High amax-> high overshooting.
	 *
	 * \param paraVmax Control-parameter maximum Velocity.
	 */
	void setVmax(S16 paraVmax) {
		vmax = paraVmax;
	}

	/**
	 * \brief Turns the controller on.
	 *
	 * Note that Motorcontroller will be started automatically when calling a move funtion.
     * Its mainly for saving process time with controllerOff/On.
	 */
	void controllerOn() {
		mon = true;
	}

	/**
	 * \brief Turns the controller off.
	 *
	 * Waits until motor is on setpoint then brake the motor.
	 * Controller intern regulation parameters are reseted.
	 */
	void controllerOff();


	/** \brief Wait until motor is on setpoint.
	*
	* This function is a blocking function. This means it will return when the motor is on setpoint.
	*/
	void waitMove() const {
	   bool go;
	   do {
	      Sleep(1);
	      //Shedule();
	      //Acquire(motor_mtx);
	      go = mgo;
	      //Release(motor_mtx);
	   } while (go);
	}

	/** \brief Checks if motor is on setpoint.
	 *
	 * @return true if motor is on setpoint.
	 */
	bool moveDone() const {
	   bool go;
	   //Acquire(motor_mtx);
	   go = mgo;
	   //Release(motor_mtx);
	   return go;
	}

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
	 * Move absolute means move motor to give position on a circle 0° - 360°
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
	 * The direction is regulated with the sign.
	 *
	 * @param pwr  The maximum allowed motor PWM power. (0 - 100)
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
	void stop(bool brake) {
		this->controllerOff();
		mot->setBrake(brake);
		mot->setPWM(0);
	}

	/**
	 * \brief Reset the motor position.
	 *
	 * Reset the motor by turning the shaft until stalling.
	 * Be aware of using this function. The position is used as absolute 0 position.
	 *
	 * @param pwr Output power, from -50 to +50, use the sign for direction.
	 *
	*/
	void resetMotorPos(S16 pwr);

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
