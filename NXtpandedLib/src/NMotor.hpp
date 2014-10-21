/*
 * Motor.hpp
 *
 *  Created on: 23.08.2014
 *      Author: Martin
 */

#ifndef MOTOR_HPP_
#define MOTOR_HPP_

#include "NIMotor.hpp"

#include "../../ecrobot/c++/device/Motor.h"

namespace nxpl {

class NMotor: public NIMotor
{
private:
	ecrobot::Motor motor;
	typedef enum mode {stopped, moving, floating};
	mode status;
public:
	/**
	 * Maximum Power (PWM) value.
	 */
	static const S8 POWER_MAX = 100;

	/**
	 * Minimum Power (PWM) value.
	 */
	static const S8 POWER_MIN = -100;

	/**
	 * Constructor (set brake by default).
	 * Note:<BR>
	 * This class must be constructed as a global object. Otherwise, a device assertion will be displayed<BR>
	 * in the LCD when the object is constructed as a non global object.
	 * @param port Motor connected port
	 * @param brake true:brake/false:float
	 * @return -
	 */
	explicit NMotor(ePortM port, bool brake = true)
			: motor(port, brake), status(brake ? stopped : floating) {
	}

	/**
	 * Destructor (stop the motor).
	 * @param -
	 * @return -
	 */
	virtual ~NMotor() {}

	/**
	 * Stop motor and set motor encoder count to 0.
	 * @param -
	 * @return -
	 */
	void resetCount() {
		motor.resetCount();
	}

	/**
	 * Get motor encoder count.
	 * @param -
	 * @return Motor encoder count in degree.
	 */
	S32 getCount() const {
		return motor.getCount();
	}

	/**
	 * Set motor encoder count.
	 * @param count Motor encoder count in degree.
	 * @return -
	 */
	void setCount(S32 count) {
		motor.setCount(count);
	}

	/**
	 * Set motor power value.
	 * @param pwm POWER_MAX to POWER_MIN
	 * @return -
	 */
	void setPower(S8 power) {
		motor.setPWM(power);
		if(power)
			status = moving;
		else
			status = motor.getBrake() ? stopped : floating;
	}

	/**
	 * Get current power value.
	 * @param -
	 * @return power set value
	 */
	S8 getPower() const {
		return motor.getPWM();
	}

	/** \brief Causes motor to stop, pretty much instantaneously.
	 * Cancels any orders in progress.
	 */
	void stop() {
		// C-API method provides shortest calling
		nxt_motor_set_speed(motor.getPort(), 0, 1);
		status = stopped;
	}

	/** \brief Causes motor to float
	 * Cancels any orders in progress.
	 */
	void flt() {
		nxt_motor_set_speed(motor.getPort(), 0, 0);
		status = floating;
	}

	/**
	 * Set brake.
	 * @param brake true:brake/false:float
	 * @return -
	 */
	void setBrake(bool brake) {
		motor.setBrake(brake);
	}

	/**
	 * Get brake status.
	 * @param -
	 * @return true:brake/false:float
	 */
	bool getBrake() const {
		return motor.getBrake();
	}

	/** Returns true if the motor is in motion.
	 *
	 * @return true if the motor is in motion.
	 */
	bool isMoving() const {
		return (status == moving);
	}

	/** Return true if motor is stopped.
	 *
	 * @return true if motor is stopped.
	 */
	bool isStopped() const {
		return (status == stopped);
	}

	/** Return true if motor is floating.
	 *
	 * @return true if motor is floating.
	 */
	bool isFloating() const {
		return (status == floating);
	}
};

} /* namespace nxpl */

#endif /* MOTOR_HPP_ */
