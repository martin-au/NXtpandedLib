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

#include "NTimer.hpp"

#include "NComSingle.hpp"
ecrobot::Usb usb;
Com::NCom comHandler(usb);
Com::NComSingle com(comHandler);


/*
#include "Motorcontroller.hpp"
ecrobot::Motor _mA(PORT_A);
Nxt::Motorcontroller motorA(&_mA, 18, 100);
*/

extern "C" {


// 1000 ms cycle
TASK(TaskMain) {
	if(!usb.isConnected()) {
		TerminateTask();
	}

	NTimer timer;

	U8 idx = 0;
	Com::NCom::comDatatype datatype;
	Com::NCom::comNModes nmode;

	timer.start();
	// receive a msg and get info
	if(com.receive(idx, datatype, nmode) > 0) {
		// ok there is a msg, who should get the msg
		if(idx == 1 && datatype == Com::NCom::typeU32 && nmode == Com::NCom::modeSingle) {
			// finally write receive msg into variable
			U32 rec = com.getData();
			cout << "Receive: " << rec << endl;
		} else {
			// for debugging
			unsigned char * pdata = com.getDataRaw();

			cout << "Error 1" << endl;
			cout << static_cast<U32>(pdata[0]) << endl;
			cout << static_cast<U32>(pdata[1]) << endl;
			cout << "                 " << endl;
			//cout << *(pdata+1) << endl;
			com.clear();
		}
	} else {
		cout << "No data" << endl;
	}

	timer.stop();
	U32 msg = timer.getLast();
	com.send(msg, 1);

	timer.reset();
	TerminateTask();
}

// 1 ms cycle
TASK(Task2) {
	usb.commHandler();
	TerminateTask();
}

}
