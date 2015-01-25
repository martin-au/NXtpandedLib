/* sample.cpp for TOPPERS/ATK(OSEK) */ 

extern "C" {
#include "../../../toppers_osek/include/kernel.h"
}

DeclareEvent(MotorAMoveDoneEvent);
DeclareEvent(MotorBMoveDoneEvent);
DeclareResource(MotorControlResource);
DeclareCounter(SysTimerCnt);

// C++ Includes and objects should be defined here.
#include "../../nxtpandedlib.h"
#include "../../src/NOstream.hpp"

NMutex motorControlMutex(MotorControlResource);

// create motor object with port A
// motor object must be created global
NMotor motorADirect(PORT_A);
NMotor motorBDirect(PORT_B);
NMotorcontroller motorA(&motorADirect, motorControlMutex, 24, 100);
NMotorcontroller motorB(&motorBDirect, motorControlMutex, 24, 100);

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
	// forward with 30
	motorA.moveRel(400, 70, false);
	motorB.moveRel(-400, 40, false);

	// do something

	motorA.waitMove();
	motorB.waitMove();

	NNxt::wait(1000);

	cout << "Abs:" << motorA.getAbsPos() << "\n";
	cout << "Rel:" << motorA.getRelPos() << "\n";
	cout.flush(true);

	// wait at this line until at position reached
	// your may try emergency stop with orange button
	motorA.moveRel(1500, 50, true);

	NNxt::wait(1000);

	// motorcontroller release control on motor and float
	motorB.off(false);
	motorA.off(false);

	NNxt::wait(5000);

	NNxt::wait(1000);
	NNxt::restart();
}

// 10 ms task
TASK(TaskMotorcontrol)
{
	motorA.process();
	motorB.process();
	TerminateTask();
}



}
