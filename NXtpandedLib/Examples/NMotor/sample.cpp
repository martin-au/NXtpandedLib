/* sample.cpp for TOPPERS/ATK(OSEK) */ 

extern "C" {
#include "../../../toppers_osek/include/kernel.h"
}

// C++ Includes and objects should be defined here.
#include "../../nxtpandedlib.h"

// create motor object with port A
// motor object must be created global
NMotor motorA(PORT_A);

extern "C" {

// nxtOSEK hook to be invoked from an ISR in category 2
void user_1ms_isr_type2(void){}

TASK(TaskMain)
{
	// forward with 30
	motorA.setPower(60);

	NNxt::wait(1000);

	// stop (break on)
	motorA.stop();

	NNxt::wait(1000);

	// reverse with 30
	motorA.setPower(-60);

	NNxt::wait(1000);

	// float
	motorA.flt();

	NNxt::wait(1000);
	NNxt::restart();
}

}
