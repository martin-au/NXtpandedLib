//
// LightSensor.cpp
//
// Copyright 2009 by Takashi Chikamasa, Jon C. Martin and Robert W. Kramer
//

#include "LightSensor.h"
using namespace ecrobot;


//=============================================================================
// Constructor
LightSensor::LightSensor(ePortS port, bool lamp)
:
Sensor(port)
{
	this->setLamp(lamp);
}

//=============================================================================
// Destructor
LightSensor::~LightSensor(void)
{
	sp_set(Sensor::getPort(), SP_DIGI0, 0); // turn off lamp
}

//=============================================================================
// get brightness (greater value means brighter)
S16 LightSensor::getBrightness(void) const
{
	return (1023 - Sensor::get());
}

//=============================================================================
// turn on/off lamp
void LightSensor::setLamp(bool lamp)
{ 
	if (lamp == true)
	{
		sp_set(Sensor::getPort(), SP_DIGI0, 1);
	}
	else
	{
		sp_set(Sensor::getPort(), SP_DIGI0, 0);
	}
}
