/* sample.cpp for TOPPERS/ATK(OSEK) */

/**
 * nxtOsek C++ NXtpandedLib
 * Author: Martin Aumair
 * License: GNU GPL on all Files
 */



// ECRobot++ API
#include "C:/cygwin/nxtOSEK/ecrobot/c++/device/Clock.h"
#include "C:/cygwin/nxtOSEK/ecrobot/c++/device/Usb.h"
#include "C:/cygwin/nxtOSEK/ecrobot/c++/device/Bluetooth.h"
//
#include "ostream.hpp"

using namespace ecrobot;
//Clock timer;
Usb usb;

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




class NCom {
private:
	ecrobot::Usb &com; // only ref because global
	typedef union header {
		struct {
			U8 disconReq :1;
			U8 datatype :4;
			U8 mode :3;
		};
		U8 asU8;
	};

public:
	enum disconReq {
		noDisconnect, disconnect
	};
	enum comDatatype {
		typeU32 = 1,
		typeS32 = 2,
		typeBool = 3,
		typeFloat = 4,
		typeChar = 5,
		typeString = 6
	};
	enum comNModes {
		modeBasic = 0,
		modeSingle = 1,
		modePackage = 2,
		modePart = 3,
		modeStream = 4
	};

	static const U8 MAX_DATA_LENGTH = ecrobot::Usb::MAX_USB_DATA_LENGTH - 2;

	NCom(ecrobot::Usb &comObject) : com(comObject) {}
	~NCom() {}

	U32 send(U8 *data, U8 idx, comDatatype datatype, comNModes nmode = modeBasic);
	U32 receive(U8 *data, U8 &idx, comDatatype &datatype, comNModes &nmode) const;
};


U32 NCom::send(U8 *data, U8 idx, comDatatype datatype, comNModes nmode) {
	header mHeader;
	mHeader.disconReq = noDisconnect;
	mHeader.datatype = datatype;
	mHeader.mode = nmode;

	data[0] = mHeader.asU8;
	data[1] = idx;
	return com.send(data, 0, ecrobot::Usb::MAX_USB_DATA_LENGTH);
}

U32 NCom::receive(U8 *data, U8 &idx, comDatatype &datatype, comNModes &nmode) const {
	memset(data, 0, Usb::MAX_USB_DATA_LENGTH);
	U32 len = 0;
	len = usb.receive(data, 0, ecrobot::Usb::MAX_USB_DATA_LENGTH);
	if(len > 0) {
		header mHeader;
		mHeader.asU8 = data[0];
		idx = data[1];
		datatype = mHeader.datatype;
		nmode = mHeader.mode;
		if(mHeader.disconReq == disconnect) {
			com.close();
		}
	}
	return len;
}



class NComSingle {
private:
	NCom &com;
	unsigned char data[ecrobot::Usb::MAX_USB_DATA_LENGTH];
public:
	NComSingle(NCom &ncom) : com(ncom) {}
	~NComSingle() {}

	U32 send(U32 n, U8 idx = 0) {
		data[2] =  (n >> 24) & 0xFF;
		data[3] =  (n >> 16) & 0xFF;
		data[4] =  (n >> 8) & 0xFF;
		data[5] =  n & 0xFF;
		return send(data, idx, NCom::typeU32, NCom::modeSingle);
	}
};

extern "C" {

//U32 len;
//unsigned char data[MAX_USB_DATA_LEN]; // first byte is preserved for disconnect request from host
//#define DISCONNECT_REQ 0xFF

TASK(TaskMain) {

	streammtx.Acquire();

	U32 len = 0;
	unsigned char data[MAX_USB_DATA_LEN]; // first byte is preserved for disconnect request from host

	if(usb.isConnected()) {
		NCom com(usb);
		NComSingle client(com);
		client.send(100);
	}

/*
	if (usb.isConnected()) // check usb connection
	{
		memset(data, 0, Usb::MAX_USB_DATA_LENGTH); // flush buffer
		len = usb.receive(data, 0, Usb::MAX_USB_DATA_LENGTH); // receive data

		if (len > 0) {
			if (data[0] == DISCONNECT_REQ) {
				usb.close(); // close usb connection
				cout << "connection closed";
			} else {
				data[0] = 0x00;
				usb.send(data, 0, len); // send data
				cout << (char*)(&data[1]);
				cout.flush();
			}
		}
	}
*/
	streammtx.Release();
	TerminateTask();
}

TASK(Task2) {
	streammtx.Acquire();
	usb.commHandler(); // USB communication handler
	streammtx.Release();
	TerminateTask();
}

}
