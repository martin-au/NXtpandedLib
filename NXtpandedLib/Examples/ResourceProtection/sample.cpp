/* sample.cpp for TOPPERS/ATK(OSEK) */ 

// fix this paths
extern "C" {
#include "../../../toppers_osek/include/kernel.h"
}

DeclareCounter(SysTimerCnt);
DeclareResource(OstreamResource);
DeclareResource(MyResource);

// C++ Includes and objects should be defined here.
#include "../../../NXtpandedLib/src/NOstream.hpp"
#include "../../../NXtpandedLib/nxtpandedlib.h"

extern "C" {

// nxtOSEK hook to be invoked from an ISR in category 2
void user_1ms_isr_type2(void){
	SleeperMonitor(); // must be called here if you use sleep function.
	StatusType ercd;

	ercd = SignalCounter(SysTimerCnt); /* Increment OSEK Alarm Counter */
	if (ercd != E_OK) {
	    ShutdownOS(ercd);
	}
}


// here we give the resource to a NXtpandedLib Mutex object
// which makes things easier
NMutex myMutex(MyResource);

TASK(TaskMain)
{
	cout << "TaskMain start\n";
	cout << NNxt::getTick() << "\n";
	cout.flush(true);

	// critical section
	//myMutex.acquire(); // use explicit acquire/release or use lock guard class
	{
	LockGuard lockGuard(myMutex); //

	int k = 0;
	for(int i=0; i<100000; i++) {
		k = i * i / k;
		if(i == 100000/2) {
			// if you take a look at FullSchedule example, here will the other task
			// get time to execute, in this case it cant because mutex is locked!
			NNxt::sleep(1);
		}
	}

	}
	//myMutex.release();

	cout << "TaskMain end\n";
	cout << NNxt::getTick() << "\n";
	cout.flush(true);
	TerminateTask();
}

TASK(Task2)
{
	myMutex.acquire();

	cout << "Task2 start\n";
	cout << NNxt::getTick() << "\n";
	cout.flush(true);

	cout << "Task2 end\n";
	cout << NNxt::getTick() << "\n";
	cout.flush(true);

	myMutex.release();

	TerminateTask();
}

}

