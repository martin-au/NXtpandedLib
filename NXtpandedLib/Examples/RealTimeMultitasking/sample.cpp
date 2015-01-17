/* sample.cpp for TOPPERS/ATK(OSEK) */ 

// fix this paths
extern "C" {
#include "../../../toppers_osek/include/kernel.h"
}

DeclareCounter(SysTimerCnt);
DeclareResource(OstreamResource);

// C++ Includes and objects should be defined here.
#include "../../../NXtpandedLib/nxtpandedlib.h"

NMutex ostreamMutex(OstreamResource);
NConsole cout(ostreamMutex); // NConsole is thread save version of console

extern "C" {

// nxtOSEK hook to be invoked from an ISR in category 2
void user_1ms_isr_type2(void){
	StatusType ercd;
	ercd = SignalCounter(SysTimerCnt); // Increment OSEK Alarm Counter every 1 ms
	if (ercd != E_OK) {
	    ShutdownOS(ercd);
	}
}

// See how task 1 with highest priority always gets executed first.
// 2 second periodic activation
TASK(Task1)
{
	cout << "Task1 - " << NNxt::getTick() << "\n";
	cout.flush(true);
	TerminateTask();
}

// 4 second periodic activation
TASK(Task2)
{
	cout << "Task2 - " << NNxt::getTick() << "\n";
	cout.flush(true);
	TerminateTask();
}

// 6 second periodic activation
TASK(Task3)
{
	cout << "Task3 - " << NNxt::getTick() << "\n";
	cout.flush(true);
	TerminateTask();

}

}

