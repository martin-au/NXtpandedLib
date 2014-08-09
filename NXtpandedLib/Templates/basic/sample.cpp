/* sample.cpp for TOPPERS/ATK(OSEK) */ 

extern "C" {
#include "../../../toppers_osek/include/kernel.h"
#include "kernel_id.h"
#include "../../../ecrobot/c/ecrobot_interface.h"
}

// C++ Includes and objects should be defined here.

extern "C" {
// startup/shutdown hooks
void ecrobot_device_initialize(void);
void ecrobot_device_terminate(void);

// nxtOSEK hook to be invoked from an ISR in category 2
void user_1ms_isr_type2(void){}

TASK(TaskMain)
{
	TerminateTask();
}

}
