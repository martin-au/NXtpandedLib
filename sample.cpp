/* sample.cpp for TOPPERS/ATK(OSEK) */

/**
 * nxtOsek C++ NXtpandedLib
 * Author: Martin Aumair
 * License: GNU GPL on all Files
 */



// ECRobot++ API
#include "C:/cygwin/nxtOSEK/ecrobot/c++/device/Clock.h"

//
#include "ostream.hpp"

using namespace ecrobot;
//Clock timer;

extern "C" {

#include "C:/cygwin/nxtOSEK/toppers_osek/include/kernel.h"
#include "kernel_id.h"
#include "C:/cygwin/nxtOSEK/ecrobot/c/ecrobot_interface.h"


DeclareResource(ostreamRes);
DeclareTask(TaskMain);
DeclareTask(Task2);
DeclareEvent(EventSleep);
DeclareEvent(EventSleepI2C);
DeclareCounter(SysTimerCnt);
DeclareAlarm(cyclic_alarm_main);
DeclareAlarm(cyclic_alarm_task2);


// hooks
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


} // extern C

#include "ostream.hpp"
mutex_t streammtx(ostreamRes);
ostream cout(streammtx);

//#include "NWidget.hpp"
#include "NNumIndicator.hpp"

// pixel set functions
#include "NLcd.hpp"

#include "C:/cygwin/GNUARM/include/c++/4.0.2/cstdlib"


#include "NTimer.hpp"



extern "C" {


TASK(TaskMain) {
	cout << "MMMMMMMMMMMMMM" << endl;
	TerminateTask();
}

TASK(Task2) {
	cout << "2222222222222" << endl;
	TerminateTask();
}

}
