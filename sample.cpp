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
DeclareEvent(EventSleep);
DeclareEvent(EventSleepI2C);
DeclareCounter(SysTimerCnt);


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
ostream cout(ostreamRes);

//#include "NWidget.hpp"
#include "NNumIndicator.hpp"

// pixel set functions
#include "NLcd.hpp"

#include "C:/cygwin/GNUARM/include/c++/4.0.2/cstdlib"


#include "NTimer.hpp"





extern "C" {


TASK(TaskMain)
{
	NTimer timer;

	//NNumIndicator indic(1, 1);

	char dest[15];
	float testfloat = -123.4567;
	S8 len = 0;
	cout << "GO!" << endl;

	timer.start();
	for(U32 i=0; i<1000; ++i) {
		//numToStr1(testfloat, dest, 2); // 29760, 104 ms, 104us
		//dest = ftoa(testfloat); // 25907, 38 ms, 38us
		len = numToStr(testfloat, dest);
	}
	timer.stop();
	//indic.setNumber(111);

	cout << "R: " << dest << endl;
	cout << "T: " << timer.getLast() << endl;
	cout << "L: " << static_cast<S32>(len) << endl;


	// show raster lines
	/*
	for(S8 i=0; i<8; i++) {
		for(S8 k=51; k<100; k++) {
			lcd.pixelMask(k, i, static_cast<U8>(1));
		}
	}
	*/
/*
	display_update();

	NLine line(&lcd, 0, 0, 99, 0);
	timer.start();
	for(S8 i=0; i<LCD::HEIGHT; ++i) {
		line.setPosition(NLine::keep, i, NLine::keep, i);
		line.show();
		line.erase();
		line.invert();
	}
	timer.stop();

	// drawer not inline
	// 54 ms / 26160

	// drawer inline
	// 46 ms / 26368
*/


	// biggest rect
	/*
	NRectangle rect(&lcd, 0, 0, 99, 63);
	t0 = timer.now();
	rect.fill();
	//rect.show();
	t1 = timer.now();
	*/

/*
	// biggest circle
	NCircle circle(&lcd, 40, 32, 20);
	NCircle circleI(&lcd, 40, 32, 10);
	NLine line(&lcd2, circle.getX(), circle.getY(), circle.centerX(), circle.centerY());

	//t0 = timer.now();
	circle.show(true);
	//circle.fill();
    //circleI.fillInvert();
	timer.sleep(1000);
	circleI.show(true);

	line.show();
	NLcd::swapLcd(lcd, lcd2);

*/

	//line.show();
	//NLcd::swapLcd(lcd2, lcd);
	//t1 = timer.now();



	// dirty virtual test


	/*
	NCircle circle(&lcd, 50, 32, 25);
	NLine line(&lcd, circle.getX(), circle.getY(), circle.getX() + circle.getWidth(), circle.getY() + circle.getHeight());
	NRectangle rect(&lcd, circle.getX(), circle.getY(), circle.getWidth(), circle.getHeight());

	NShape *obj[] = {&circle, &line, &rect};
	t0 = timer.now();
	for (S32 i=0; i<3; ++i) {
		obj[i]->show();
	}
	t1 = timer.now();
	// 27312

	*/

	/*
	dtIndic.setNumber(sizeof(NRectangle));

	dtIndic.show();
	dtIndic.getBuddy()->show(true);
	*/

	TerminateTask();
}

}
