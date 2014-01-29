/* sample.cpp for TOPPERS/ATK(OSEK) */

/**
 * nxtOsek C++ NXtpandedLib
 * Author: Martin Aumair
 * License: GNU GPL on all Files
 */


extern "C" {

#include "C:/cygwin/nxtOSEK/toppers_osek/include/kernel.h"
#include "kernel_id.h"
#include "C:/cygwin/nxtOSEK/ecrobot/c/ecrobot_interface.h"
#include "C:/cygwin/nxtOSEK/ecrobot/c/rtoscalls.h"

// System
DeclareEvent(EventSleep);
DeclareEvent(EventSleepI2C);
DeclareCounter(SysTimerCnt);

// User
DeclareResource(ostreamRes);
DeclareResource(motorARes);
DeclareTask(TaskMain);
DeclareTask(Task2);
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

// C++ Includes, Globals

#include "ostream.hpp"
mutex_t streammtx(ostreamRes);
ostream cout(streammtx);


#include "Motorcontroller.hpp"
ecrobot::Motor _mA(PORT_A);
Nxt::Motorcontroller motorA(&_mA, 18, 100);

extern "C" {

//U32 len;
//unsigned char data[MAX_USB_DATA_LEN]; // first byte is preserved for disconnect request from host
//#define DISCONNECT_REQ 0xFF

TASK(TaskMain) {
	cout << "Controller On" << endl;
	motorA.controllerOn();
	cout << "Motor Go" << endl;
	GetResource(motorARes);
	motorA.moveRel(180, 50, false);
	ReleaseResource(motorARes);
	cout << "Moving" << endl;
	TerminateTask();
}

TASK(Task2) {
	GetResource(motorARes);
	motorA.process();
	ReleaseResource(motorARes);
	TerminateTask();
}

}
