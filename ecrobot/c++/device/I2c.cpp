//
// I2c.cpp
//
// Copyright 2009 by Takashi Chikamasa, Jon C. Martin and Robert W. Kramer
//

#include "I2c.h"
using namespace ecrobot;


//=============================================================================
// Constructor
I2c::I2c(ePortS port, ePower power)
:
mPort(port)
{
	AssertDeviceConstructor("I2c Assert");
	nxt_avr_set_input_power(mPort, power);
	i2c_enable(mPort);
	// i2c_enable(mPort, 1);
}

//=============================================================================
// Destructor
I2c::~I2c(void)
{
	nxt_avr_set_input_power(mPort, POWER_OFF);
	i2c_disable(mPort);
}

//=============================================================================
// send data
bool I2c::send(U32 regAddr, U8* data, U32 length)
{
	SleepI2C(mPort); // sleep running Task if I2C was busy
	U8 ret = i2c_start_transaction(mPort,1,regAddr,1,data,length,1/* write */);
	SleepI2C(mPort); // sleep running Task if I2C was busy

	return static_cast<bool>(!ret);
}

//=============================================================================
// receive data
bool I2c::receive(U32 regAddr, U8* data, U32 length) const
{
	SleepI2C(mPort); // sleep running Task if I2C was busy
	U8 ret = i2c_start_transaction(mPort,1,regAddr,1,data,length,0/* read */);
	SleepI2C(mPort); // sleep running Task if I2C was busy

	return static_cast<bool>(!ret);
}

//=============================================================================
// send data
bool I2c::send(U32 address, SINT regAddr, U8* data, U32 length)
{
	SleepI2C(mPort); // sleep running Task if I2C was busy
	U8 ret = i2c_start_transaction(mPort,address,regAddr,1,data,length,1/* write */);
	SleepI2C(mPort); // sleep running Task if I2C was busy

	return static_cast<bool>(!ret);
}

//=============================================================================
// receive data
bool I2c::receive(U32 address, SINT regAddr, U8* data, U32 length) const
{
	SleepI2C(mPort); // sleep running Task if I2C was busy
	U8 ret = i2c_start_transaction(mPort,address,regAddr,1,data,length,0/* read */);
	SleepI2C(mPort); // sleep running Task if I2C was busy

	return static_cast<bool>(!ret);
}

//=============================================================================
// send data
bool I2c::send(U32 address, SINT regAddr, SINT nInternalAddressBytes, U8* data, U32 length)
{
	SleepI2C(mPort); // sleep running Task if I2C was busy
	U8 ret = i2c_start_transaction(mPort,address,regAddr,nInternalAddressBytes,data,length,1/* write */);
	SleepI2C(mPort); // sleep running Task if I2C was busy

	return static_cast<bool>(!ret);
}

//=============================================================================
// receive data
bool I2c::receive(U32 address, SINT regAddr, SINT nInternalAddressBytes, U8* data, U32 length) const
{
	SleepI2C(mPort); // sleep running Task if I2C was busy
	U8 ret = i2c_start_transaction(mPort,address,regAddr,nInternalAddressBytes,data,length,0/* read */);
	SleepI2C(mPort); // sleep running Task if I2C was busy

	return static_cast<bool>(!ret);
}

