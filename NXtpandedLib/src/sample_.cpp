/* sample.cpp for TOPPERS/ATK(OSEK) */

/**
 * nxtOsek C++ NXtpandedLib
 * Author: Martin Aumair
 * License: GNU GPL on all Files
 */


extern "C" {

#include "../../toppers_osek/include/kernel.h"
#include "kernel_id.h"
#include "../../ecrobot/c/ecrobot_interface.h"
#include "../../ecrobot/c/rtoscalls.h"
#include "../../lejos_nxj/src/nxtvm/platform/nxt/mytypes.h"

// System
DeclareEvent(EventSleep);
DeclareEvent(EventSleepI2C);
DeclareCounter(SysTimerCnt);
// User
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

using namespace nxpl;

#include "NTimer.hpp"

NTimer timer;

extern "C" {

TASK(TaskMain) {

	TerminateTask();
}

}

nxpl::NLabel absPosLabel(NTextBox(NCursor(), 5));

extern "C" {

// 10 ms cycle
TASK(Task2) {
	/*
	absPosLabel.setNumber(motorA.getAbsPos());
	absPosLabel.show(true);
	*/
	motorA.process();
	TerminateTask();
}

}
