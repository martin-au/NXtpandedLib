/*
 * NIMotor.hpp
 *
 *  Created on: 23.08.2014
 *      Author: Martin
 */

#ifndef NIMOTOR_HPP_
#define NIMOTOR_HPP_

/** \brief A interface class for motors
 *
 */
class NIMotor
{
public:
	virtual ~NIMotor() {}

	/**
	 * Set motor encoder count to 0.
	 * @param -
	 * @return -
	 */
	virtual void resetCount() = 0;

	/**
	 * Get motor encoder count.
	 * @param -
	 * @return Motor encoder count in degree.
	 */
	virtual S32 getCount() const = 0;

	/**
	 * Set motor power value.
	 * @param pwm PWM_MAX to PWM_MIN
	 * @return -
	 */
	virtual void setPower(S8 power) = 0;

	/**
	 * Get current power value.
	 * @param -
	 * @return power set value
	 */
	virtual S8 getPower() const = 0;

	/** \brief Causes motor to stop, pretty much instantaneously.
	 */
	virtual void stop() = 0;

	/** \brief Causes motor to float
	 */
	virtual void flt() = 0;

	/**
	 * Set brake.
	 * @param brake true:brake/false:float
	 * @return -
	 */
	virtual void setBrake(bool brake) = 0;

	/**
	 * Get brake status.
	 * @param -
	 * @return true:brake/false:float
	 */
	virtual bool getBrake() const = 0;

	/** Returns true if the motor is in motion.
	 *
	 * @return true if the motor is in motion.
	 */
	virtual bool isMoving() const = 0;


	/** Return true if motor is stopped.
	 *
	 * @return true if motor is stopped.
	 */
	virtual bool isStopped() const = 0;

	/** Return true if motor is floating.
	 *
	 * @return true if motor is floating.
	 */
	virtual bool isFloating() const = 0;
};

#endif /* NIMOTOR_HPP_ */
