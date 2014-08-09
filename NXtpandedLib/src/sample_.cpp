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
DeclareResource(ostreamRes);
DeclareResource(motorRes);
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


}; // extern C

// C++ Includes, Globals


#include "NOstream.hpp"
nxpl::mutex_t streammtx(ostreamRes);
nxpl::NOstream cout(streammtx);


#include "NTimer.hpp"
#include "Motorcontroller.hpp"

nxpl::mutex_t motorControlMtx(motorRes);
ecrobot::Motor motorADirect(PORT_A);
nxpl::Motorcontroller motorA(&motorADirect, motorControlMtx, 24, 100); // 18 100

#include "NLabel.hpp"

extern "C" {

using namespace nxpl;

TASK(TaskMain) {

	NTimer timer;

	cout << "Reset now\n";
	motorA.resetMotorPos(30, true);
	cout << "1.Abs: " << motorA.getAbsPos() << "\n";
	cout << "1.Rel: " << motorA.getRelPos() << "\n";

	timer.sleep(1000);

	motorA.moveAbs(180, 40, true);
	cout << "2.Abs: " << motorA.getAbsPos() << "\n";
	cout << "2.Rel: " << motorA.getRelPos() << "\n";

	cout.flush(true);

	TerminateTask();
}


nxpl::NLabel absPosLabel(nxpl::NTextBox(nxpl::NCursor(), 5));

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
