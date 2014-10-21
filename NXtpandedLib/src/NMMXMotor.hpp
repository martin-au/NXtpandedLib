/*
 * NMMXMotor.hpp
 *
 *  Created on: 23.08.2014
 *      Author: Martin
 */

#ifndef NMMXMOTOR_HPP_
#define NMMXMOTOR_HPP_

#include "NIMotor.hpp"
#include "../../ecrobot/c/ecrobot_mindsensors.h"
#include "../../ecrobot/c++/device/I2c.h"

enum MMXMPorts
{
	MMXPortM1 = 0x01, MMXPortM2 = 0x02
};

namespace nxpl {

class NMMXMotor : public NIMotor
{
private:
	bool checkBit(U8 var, U8 pos)
	{
		return (var) & (1 << (pos));
	}

	ecrobot::I2c i2c;

	ePortS nxtSPort;
	MMXMPorts mmxMPort;
	U8 i2cAddr;

	bool brake;
	S8 power;

	// helper functions
	bool sendI2c(int regAddr, U8* data, U32 length)
	{
		return i2c.send(i2cAddr, regAddr, data, length);
	}

	bool receiveI2c(int regAddr, U8* data, U32 length) const
	{
		return i2c.receive(i2cAddr, regAddr, data, length);
	}

	bool issueCommand(char command)
	{
		return i2c.sendByte(MMX_CMD::COMMAND, static_cast<U8>(command));
	}

public:
	static const U8 DEFAULT_I2C_ADDR = 0x03;

	NMMXMotor(ePortS nxtSensorPort, MMXMPorts mmxMotorPort, bool brake = true,
			U8 i2cAddress = DEFAULT_I2C_ADDR)
			: i2c(nxtSensorPort), nxtSPort(nxtSensorPort), mmxMPort(mmxMotorPort),
			  i2cAddr(i2cAddress), brake(brake), power(0), lastCount(-9999), count(-9999)
	{

	}

	virtual ~NMMXMotor()
	{
	}

	/**
	 * Stop motor and set motor encoder count to 0.
	 * @param -
	 * @return -
	 */
	void resetCount()
	{
		// Reset the encoder for motor 1 or motor 2
		char code = (mmxMPort == MMXPortM1) ? 'r' : 's';
		issueCommand(code);
	}

	mutable S32 lastCount;
	mutable S32 count;

	/**
	 * Get motor encoder count.
	 * @param -
	 * @return Motor encoder count in degree.
	 */
	S32 getCount() const
	{

		lastCount = count;

		U8 data[4] = {0};

		U8 location =
				(mmxMPort == MMX_Motor_1) ?
						MMX_CMD::MMX_POSITION_M1 : MMX_CMD::MMX_POSITION_M2;

		// receiveI2c(0x62, data, 4);

		// ecrobot_read_i2c(port_id, i2caddr, 0x66, data, 4);

		i2c_start_transaction(PORT_1, 0x03, MMX_CMD::MMX_POSITION_M1, 1, data, 4, 0);

		//count = count | (U32) data[0] | ((U32) data[1]) << 8
		//		| ((U32) data[2]) << 16 | ((U32) data[3]) << 24;
		count = (S32) data[4];

		return lastCount;

		// return ecrobot_get_MMX_count(nxtSPort, i2cAddr, mmxMPort);
	}

	/**
	 * Set motor encoder count.
	 * @param count Motor encoder count in degree.
	 * @return -
	 */
//	void setCount(S32 count) {
	//	// TODO Set count on mmx?
	//}
	/**
	 * Set motor power value.
	 * @param pwm POWER_MAX to POWER_MIN
	 * @return -
	 */
	void setPower(S8 power)
	{
		// TODO make constanst
		this->power = power > 100 ? 100 : power;
		this->power = power < -100 ? -100 : power;
		ecrobot_set_MMX_speed(nxtSPort, i2cAddr, mmxMPort, this->power, brake);
	}

	/**
	 * Get current power value.
	 * @param -
	 * @return power set value
	 */
	S8 getPower() const
	{
		return power;
	}

	/** \brief Causes motor to stop, pretty much instantaneously.
	 * Cancels any orders in progress.
	 */
	void stop()
	{
		ecrobot_set_MMX_stop(nxtSPort, i2cAddr, mmxMPort, true);
	}

	/** \brief Causes motor to float
	 * Cancels any orders in progress.
	 */
	void flt()
	{
		ecrobot_set_MMX_stop(nxtSPort, i2cAddr, mmxMPort, false);
	}

	/**
	 * Set brake.
	 * @param brake true:brake/false:float
	 * @return -
	 */
	void setBrake(bool brake)
	{
		this->brake = brake;
		setPower(power);
	}

	/**
	 * Get brake status.
	 * @param -
	 * @return true:brake/false:float
	 */
	bool getBrake() const
	{
		return brake;
	}

	/**
	 *  * 	Bit 0: 1 Motor is programmed to move at a fixed speed
	 *  Bit 1: Motor is Ramping (up or down).
	 *  Bit 2: Motor is powered.
	 *  Bit 3: Positional Control is ON. The motor is either moving
	 *         towards desired encoder position or holding its position.
	 *  Bit 4: Motor is in Brake mode.
	 *  Bit 5: Motor is overloaded.
	 *  Bit 6: Motor is in timed mode. This bit is 1 while the motor
	 is programmed to move for given duration.
	 *  Bit 7: Motor is stalled.
	 * @return
	 */

	/** Returns true if the motor is in motion.
	 *
	 * @return true if the motor is in motion.
	 */
	bool isMoving() const
	{
		U8 status = MMX_MotorStatus(nxtSPort, i2cAddr, mmxMPort);
		return (status & 0x69) > 0;
		// 1101001
	}

	/** Return true if motor is stopped.
	 *
	 * @return true if motor is stopped.
	 */
	bool isStopped() const
	{
		return (!isMoving());
	}

	/** Return true if motor is floating.
	 *
	 * @return true if motor is floating.
	 */
	bool isFloating() const
	{
		return MMX_MotorStatus(nxtSPort, i2cAddr, mmxMPort) == 0;
	}

	U8 getStatus() const
	{
		return MMX_MotorStatus(nxtSPort, i2cAddr, mmxMPort);
	}

private:
	struct MMX_CMD
	{
		static const U8 CONTROL_SPEED = 0x01;
		static const U8 CONTROL_RAMP = 0x02;
		static const U8 CONTROL_RELATIVE = 0x04;
		static const U8 CONTROL_TACHO = 0x08;
		static const U8 CONTROL_BRK = 0x10;
		static const U8 CONTROL_ON = 0x20;
		static const U8 CONTROL_TIME = 0x40;
		static const U8 CONTROL_GO = 0x80;

		static const U8 COMMAND = 0x41;
		static const U8 VOLTAGE = 0x41;

		static const U8 SETPT_M1 = 0x42;
		static const U8 SPEED_M1 = 0x46;
		static const U8 TIME_M1 = 0x47;
		static const U8 CMD_B_M1 = 0x48;
		static const U8 CMD_A_M1 = 0x49;

		static const U8 SETPT_M2 = 0x4A;
		static const U8 SPEED_M2 = 0x4E;
		static const U8 TIME_M2 = 0x4F;
		static const U8 CMD_B_M2 = 0x50;
		static const U8 CMD_A_M2 = 0x51;

		/*
		 * Read registers.
		 */
		static const U8 MMX_POSITION_M1 = 0x62;
		static const U8 MMX_POSITION_M2 = 0x66;
		static const U8 MMX_STATUS_M1 = 0x72;
		static const U8 MMX_STATUS_M2 = 0x73;
		static const U8 MMX_TASKS_M1 = 0x76;
		static const U8 MMX_TASKS_M2 = 0x77;

		static const U8 MMX_ENCODER_PID = 0x7A;
		static const U8 MMX_SPEED_PID = 0x80;
		static const U8 MMX_PASS_COUNT = 0x86;
		static const U8 MMX_TOLERANCE = 0x87;

		/*
		 * Next action related constants
		 */
		// stop and let the motor coast.
		static const U8 MMX_Next_Action_Float = 0x00;
		// apply brakes, and resist change to tachometer
		static const U8 MMX_Next_Action_Brake = 0x01;
		// apply brakes, and restore externally forced change to tachometer
		static const U8 MMX_Next_Action_BrakeHold = 0x02;

		/*
		 * Direction related constants
		 */
		static const U8 MMX_Direction_Forward = 0x01;
		static const U8 MMX_Direction_Reverse = 0x00;

		/*
		 * Tachometer related constants
		 */
		static const U8 MMX_Move_Relative = 0x01;
		static const U8 MMX_Move_Absolute = 0x00;

		static const U8 MMX_Completion_Wait_For = 0x01;
		static const U8 MMX_Completion_Dont_Wait = 0x00;
	};
};

} /* namespace nxpl */

#endif /* NMMXMOTOR_HPP_ */
