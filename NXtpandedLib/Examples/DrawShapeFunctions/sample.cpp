/* sample.cpp for TOPPERS/ATK(OSEK) */ 

extern "C" {
#include "C:/cygwin/nxtOSEK/toppers_osek/include/kernel.h"
#include "kernel_id.h"
#include "C:/cygwin/nxtOSEK/ecrobot/c/ecrobot_interface.h"
}

DeclareTask(TaskMain);

// C++ Includes and objects should be defined here.
#include "../../../NXtpandedLib/src/LcdDrawer.hpp"

extern "C" {
// startup/shutdown hooks
void ecrobot_device_initialize(void);
void ecrobot_device_terminate(void);

// nxtOSEK hook to be invoked from an ISR in category 2
void user_1ms_isr_type2(void){}


using namespace nxpl;

TASK(TaskMain)
{
	NLcd lcd;

	drawLine(lcd, NPoint(0, 0), NPoint(99, 63));
	drawRectangleFilled(lcd, NPixelBox(NPoint(10, 10), 20, 30));
	drawRectangleFilled(lcd, NPixelBox(NPoint(20, 20), 20, 30), DrawOpt::invert());
	drawCircleFilled(lcd, NPoint(70, 30), 13);

 	display_update();
 	TerminateTask();
}

}
