/* sample.cpp for TOPPERS/ATK(OSEK) */ 

// fix this paths
extern "C" {
#include "../../../toppers_osek/include/kernel.h"
#include "kernel_id.h"
#include "../../../ecrobot/c/ecrobot_interface.h"
}

DeclareCounter(SysTimerCnt);
DeclareResource(OstreamResource);
DeclareResource(MyResource);

// C++ Includes and objects should be defined here.
// fix this paths also
#include "../../../NXtpandedLib/src/NOstream.hpp"
#include "../../../NXtpandedLib/src/NTimer.hpp"
#include "../../../NXtpandedLib/src/Mutex.hpp"

nxpl::mutex_t ostreamMtx(OstreamResource);
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


// here we give the resource to a NXtpandedLib Mutex object
// which makes things easier
nxpl::mutex_t myMutex(MyResource);

TASK(TaskMain)
{
	nxpl::NTimer timer;
	cout << "TaskMain start\n";
	cout << systick_get_ms() << nxpl::endl;
	display_update();

	//myMutex.acquire();
	{
	nxpl::LockGuard lockGuard(myMutex);

	int k = 0;
	for(int i=0; i<100000; i++) {
		k = i * i / k;
		if(i == 100000/2) {
			timer.sleep(1);
		}
	}

	}
	//myMutex.release();

	cout << "TaskMain end\n";
	cout << systick_get_ms() << nxpl::endl;
	display_update();
	TerminateTask();
}

TASK(Task2)
{
	myMutex.acquire();

	cout << "Task2 start\n";
	cout << systick_get_ms() << nxpl::endl;
	display_update();

	cout << "Task2 end\n";
	cout << systick_get_ms() << nxpl::endl;
	display_update();

	myMutex.release();

	TerminateTask();
}

}

