/* sample.cpp for TOPPERS/ATK(OSEK) */ 

extern "C" {
#include "../../../toppers_osek/include/kernel.h"
}

// C++ Includes and objects should be defined here.
#include "../../nxtpandedlib.h"

extern "C" {

// nxtOSEK hook to be invoked from an ISR in category 2
void user_1ms_isr_type2(void){}

TASK(TaskMain)
{
	NLine::draw(NPoint(0, 0), NPoint(99, 63));
	NRectangleFilled::draw(NPixelBox(NPoint(10, 10), 20, 30));
	NRectangleFilled::draw(NPixelBox(NPoint(20, 20), 20, 30), DrawOpt::invert());
	NCircleFilled::draw(NPoint(70, 30), 13);

	NLcd::update();

 	NNxt::wait(10000);
 	NNxt::restart();
}

}
