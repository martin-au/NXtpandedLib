/* sample.cpp for TOPPERS/ATK(OSEK) */ 

// fix this paths
extern "C" {
#include "../../../toppers_osek/include/kernel.h"
#include "kernel_id.h"
#include "../../../ecrobot/c/ecrobot_interface.h"
}

DeclareCounter(SysTimerCnt);
DeclareTask(Task1);
DeclareTask(Task2);
DeclareTask(Task3);

DeclareResource(OstreamResource);

// C++ Includes and objects should be defined here.
// fix this paths also
#include "../../../NXtpandedLib/src/NOstream.hpp"

nxpl::mutex_t ostreamMtx(OstreamResource);
nxpl::NOstream cout(ostreamMtx);

extern "C" {
// startup/shutdown hooks
void ecrobot_device_initialize(void);
void ecrobot_device_terminate(void);

// nxtOSEK hook to be invoked from an ISR in category 2
void user_1ms_isr_type2(void){
	StatusType ercd;

	ercd = SignalCounter(SysTimerCnt); /* Increment OSEK Alarm Counter */
	if (ercd != E_OK) {
	    ShutdownOS(ercd);
	}
}

// 2 second activation
TASK(Task1)
{
	cout << "Task1 - " << systick_get_ms() << nxpl::endl;
	display_update();
	TerminateTask();
}

// 4 second activation
TASK(Task2)
{
	cout << "Task2 - " << systick_get_ms() << nxpl::endl;
	display_update();
	TerminateTask();
}

// 6 second activation
TASK(Task3)
{
	cout << "Task3 - " << systick_get_ms() << nxpl::endl;
	display_update();
	TerminateTask();

}

}

