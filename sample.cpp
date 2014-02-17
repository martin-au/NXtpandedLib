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

#include "NOstream.hpp"
mutex_t streammtx(ostreamRes);
NOstream cout(streammtx);

#include "NTimer.hpp"

#include "NLabel.hpp"
#include "NPairBox.hpp"

/*
#include "NComSingle.hpp"
ecrobot::Usb usb;
Com::NCom comHandler(usb);
Com::NComSingle com(comHandler);
*/

/*
#include "Motorcontroller.hpp"
ecrobot::Motor _mA(PORT_A);
Nxt::Motorcontroller motorA(&_mA, 18, 100);
*/

extern "C" {

//Com::NCom::comDatatype state = Com::NCom::typeU32;
//U8 state = static_cast<U8>(Com::NCom::typeU32);
U8 state = 0;


NLabel label1;
NLabel label2("state:");

NPairBox<NLabel, NLabel>  box(&label2, &label1, 2, 2, NAlignment::right());


// 1000 ms cycle
TASK(TaskMain) {
	streammtx.acquire();
	box.sec->setNumber(state);
	streammtx.release();

	/* USB EXAMPLE
	if(!usb.isConnected()) {
		TerminateTask();
	}

	U8 idx = 0;
	Com::NCom::comDatatype datatype;
	Com::NCom::comNModes nmode;

	S32 datalen = com.receive(idx, datatype, nmode);
	// receive a msg and get info
	if(datalen > 0) {
		// ok there is a msg, who should get the msg
		switch(datatype) {
		// finally write receive msg into variable
		case Com::NCom::typeU32 :
		{
			U32 rec = com.getDataU32();
			cout << "Receive: " << rec << endl;
			break;
		}
		case Com::NCom::typeS32 :
		{
			if(nmode == Com::NCom::modeSingle) {
				S32 rec = com.getDataS32();
				cout << "Receive: " << rec << endl;
			} else if(nmode == Com::NCom::modePackage) {
				S32 *data = NULL;
				S32 len = com.getDataPackageS32(data);
				if(len > 0 && data != NULL) {
					cout << "Receive:" << endl;
					for(S16 i=0; i<len; ++i) {
						cout << "Package: " << data[i] << endl;
					}
				}
				delete[] data;
			}
			break;
		}
		case Com::NCom::typeBool:
		{
			bool rec = com.getDataBool();
			cout << "Receive: " << rec << endl;
			break;
		}
		case Com::NCom::typeFloat:
		{
			float rec = com.getDataFloat();
			cout << "Receive: " << rec << endl;
			break;
		}
		case Com::NCom::typeChar:
		{
			char rec = com.getDataChar();
			cout << "Receive: " << rec << endl;
			break;
		}
		case Com::NCom::typeString:
		{
			NString rec = com.getDataString();
			cout << "Receive: " << rec << endl;
			break;
		}
		default:
		{	// for debugging
			unsigned char * pdata = com.getDataRaw();

			cout << "Error 1" << endl;
			cout << static_cast<U32>(pdata[0]) << endl;
			cout << static_cast<U32>(pdata[1]) << endl;
			cout << "                 " << endl;
			//cout << *(pdata+1) << endl;
			com.discard();
		}
		}

	} else {
		//cout << "No data" << endl;
	}

	switch(state) {
	case Com::NCom::typeU32:
	{
		U32 msg = 123456;
		com.send(msg, 1);
	}
	break;
	case Com::NCom::typeS32:
	{
		S32 msg = -123456;
		com.send(msg, 1);
	}
	break;
	case Com::NCom::typeBool:
	{
		bool msg = true;
		com.send(msg, 1);
	}
	break;
	case Com::NCom::typeFloat:
	{
		float msg = -123.456;
		com.send(msg, 1);
	}
	break;
	case Com::NCom::typeChar:
	{
		char msg = 'M';
		com.send(msg, 1);
	}
	break;
	case Com::NCom::typeString:
	{
		NString msg("Hallo Welt");
		com.send(msg, 1);
	}
	break;
	case 7:
	{
		U32 pack[4];
		pack[0] = 1;
		pack[1] = 2;
		pack[2] = 3;
		pack[3] = 4;
		com.send(pack, 1, 4);
	}
	break;
	default: break;
	}
	*/

	++state;


	TerminateTask();
}

// 300 ms cycle
TASK(Task2) {
	streammtx.acquire();
	box.show(true);
	streammtx.release();
	//cout.flush();
	//usb.commHandler();
	TerminateTask();
}

}
