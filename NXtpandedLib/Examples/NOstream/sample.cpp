/* sample.cpp for TOPPERS/ATK(OSEK) */ 

// fix this paths
extern "C" {
#include "../../../toppers_osek/include/kernel.h"
#include "kernel_id.h"
#include "../../../ecrobot/c/ecrobot_interface.h"
}


DeclareCounter(SysTimerCnt);
DeclareResource(OstreamResource);

// C++ Includes and objects should be defined here.
// fix this paths also
#include "../../../NXtpandedLib/src/NOstream.hpp"
#include "../../../NXtpandedLib/src/NTimer.hpp"

nxpl::NMutex ostreamMtx(OstreamResource);
nxpl::NOstream cout(ostreamMtx);

extern "C" {
// startup/shutdown hooks
void ecrobot_device_initialize(void);
void ecrobot_device_terminate(void);

// nxtOSEK hook to be invoked from an ISR in category 2
void user_1ms_isr_type2(void){
	SleeperMonitor();
	StatusType ercd;

	ercd = SignalCounter(SysTimerCnt); /* Increment OSEK Alarm Counter */
	if (ercd != E_OK) {
	    ShutdownOS(ercd);
	}
}

TASK(TaskMain)
{
	cout << "Hello world" << nxpl::endl;
	cout << "Hello world\n";
	cout << "Hello world" << "\n";
	cout.flush(true);

	TerminateTask();
}

}

