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


}; // extern C

// C++ Includes, Globals


#include "NOstream.hpp"
nxpl::mutex_t streammtx(ostreamRes);
//nxpl::NOstream cout(streammtx);


#include "NLabel.hpp"
#include "NPairBox.hpp"
#include "NTimer.hpp"

/*
#include "NLabel.hpp"
#include "NPairBox.hpp"
#include "NLcdView.hpp"
*/

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




//Com::NCom::comDatatype state = Com::NCom::typeU32;
//U8 state = static_cast<U8>(Com::NCom::typeU32);
U8 state = 0;

/*
NLabel label1;
NLabel label2("state:");

NPairBox<NLabel, NLabel>  box(&label2, &label1, 2, 2, NAlignment::right());
*/

// 1000 ms cycle

/*
#include "NLcdView.hpp"
#include "NRectangle.hpp"
#include "NCircle.hpp"
#include "NLine.hpp"
*/

extern "C" {

using namespace nxpl;

TASK(TaskMain) {




	/*
	NLabel test("(test)", NTextBox(NCursor(5, 0), 7));
	test.show(true);

	NLabel toRight(">>", NTextBox(NCursor(14, 3), 2));
	NLabel toLeft("<<", NTextBox(NCursor(0, 3), 2));
	NLabel lastSite("(main)", NTextBox(NCursor(5, 3), 7));

	NOstream cout(streammtx, NTextBox(NCursor(3, 0), 14, 8));

	NLcd lcd;
	NRectangleFilled rectangle(lcd, NPixelBox(NPoint(20, 20), 30, 30));

	NLcdView mainSite(5);
	mainSite.add(&toRight);
	mainSite.add(&toLeft);
	mainSite.add(&lastSite);

	NLcdView rightSite(2);
	rightSite.add(&toLeft);
	rightSite.add(&cout);

	NLcdView leftSite(2);
	leftSite.add(&toRight);
	leftSite.add(&rectangle);

	NTimer timer;

	const S8 mainSiteId = 0;
	const S8 leftSiteId = -1;
	const S8 rightSiteId = 1;

	mainSite.show(true);
	test.show(true);
	int site = mainSiteId;
	*/



	/*
	nxpl::NTimer timer;
	cout << "TaskMain start\n";
	cout << systick_get_ms() << nxpl::endl;
	display_update();


	int k = 0;
	for (int i = 0; i < 10000; i++) {
		k = i * i / k;
		timer.wait(1);
	}

	cout << "TaskMain end\n";
	cout << systick_get_ms() << nxpl::endl;
	display_update();
	*/


	/*
	nxpl::NTimer timer;

	nxpl::NLabel labelHello("Hello", nxpl::NTextBox(nxpl::NCursor(6, 5), 5, 1));
	nxpl::NLabel labelNumber(nxpl::NTextBox(nxpl::NCursor(0, 0), 5, 1));
	nxpl::NPairBox box(&labelHello, &labelNumber);

	int i = 0;
	while(true) {
		static_cast<nxpl::NLabel*>(box.sec)->setNumber(i);
		switch(i) {
		case 0:
			box.align2Main(nxpl::NAlignment::top());
			break;
		case 1:
			box.align2Main(nxpl::NAlignment::right());
			break;
		case 2:
			box.align2Main(nxpl::NAlignment::bottom());
			break;
		case 3:
			box.align2Main(nxpl::NAlignment::left());
			break;
		}
		box.show(true);
		timer.wait(1000);
		++i;
		if(i > 3) i = 0;
	}

*/
	/*
	nxpl::NLabel label1("time:");
	nxpl::NLabel label2;
	nxpl::NPairBox<nxpl::NLabel, nxpl::NLabel> box(&label1, &label2, 0, 0, nxpl::NAlignment::right());

	nxpl::NWidget *pbase = &label1;

	timer.start();
	// 41 ms / 26448B non virtual
	// 41 ms / 26816B virtual
	// 42 ms / 26816B virtual + baseclass call
	//
    // virtual: - program size increase 368B
    //	 - RT increase + 1/1000 ms per base call
   	// Conclusion: make now virtual and implement later a user choice for change
   	//          to non virtual during compilation

	for(int i=0; i<1000; i++) {
		label1.setText("time:"); // force update
		pbase->show();
	}

	box.sec->setNumber(timer.stop());
	box.show(true);
	*/

	/* view test
	nxpl::NLabel label1("label1", 0, 0);
	nxpl::NLabel label2(sizeof(label1), 0, 1);

	nxpl::NLcdView view1;
	nxpl::NLcdView view2;

	cout << "Hello World";

	view1.add(&label1);
	view1.add(&label2);
	view2.add(&cout);

	view1.show(true);

	timer.wait(2000);

	nxpl::NLcdView::swap(view1, view2, true);

	timer.wait(2000);

	view1.remove(&label1);

	nxpl::NLcdView::swap(view2, view1, true);
*/

	/*
	// 0 - os 25344
	timer.start();
	int *vec = new int[3001];
	vec[0] = state;
	for (int i = 1; i < 3001; i++) {
		vec[i] = i;
	}
	cout << timer.stop() << "\n";
	delete[] vec;
	*/

	//cout.flush(true);

	/* LABEL TEST
	streammtx.acquire();
	box.sec->setNumber(state);
	streammtx.release();
	*/

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
	/*
nxpl::NTimer timer;

	cout << "Task2 start" << nxpl::endl;
	//cout << systick_get_ms() << nxpl::endl;
	display_update();
	timer.wait(1000);
	cout << "   " << nxpl::endl;
	//cout << systick_get_ms() << nxpl::endl;

	display_update();

*/
	/*
	streammtx.acquire();
	box.show(true);
	streammtx.release();
	*/

	//cout.flush();
	//usb.commHandler();
	TerminateTask();
}

}
