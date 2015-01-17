/* sample.cpp for TOPPERS/ATK(OSEK) */ 

extern "C" {
#include "../../../toppers_osek/include/kernel.h"
}

DeclareCounter(SysTimerCnt);

// C++ Includes and objects should be defined here.
// fix this paths also
#include "../../../NXtpandedLib/src/NOstream.hpp"
#include "../../nxtpandedlib.h"

extern "C" {

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
	cout << "TaskMain start\n";
	cout << NNxt::getTick() << endl;
	NLcd::update();

	// do something
	int k = 0;
	for(int i=0; i<10000; i++) {
		k = i * i / k;
		NNxt::wait(1);
	}

	cout << "TaskMain end\n";
	cout << NNxt::getTick() << endl;
	NLcd::update();
	TerminateTask();
}

// Task 2 will interrupt TaskMain multiple times because it has fixed cycle time and
// higher priority (see .oil file)
TASK(Task2)
{
	cout << "Task2 start\n";
	cout << NNxt::getTick() << endl;
	NLcd::update();

	cout << "Task2 end\n";
	cout << NNxt::getTick() << endl;
	NLcd::update();
	TerminateTask();
}

}

