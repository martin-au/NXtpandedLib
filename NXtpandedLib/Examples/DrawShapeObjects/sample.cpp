/* sample.cpp for TOPPERS/ATK(OSEK) */ 

extern "C" {
#include "C:/cygwin/nxtOSEK/toppers_osek/include/kernel.h"
#include "kernel_id.h"
#include "C:/cygwin/nxtOSEK/ecrobot/c/ecrobot_interface.h"
}

DeclareTask(TaskMain);

// C++ Includes and objects should be defined here.
#include "../../../NXtpandedLib/src/NLine.hpp"
#include "../../../NXtpandedLib/src/NRectangle.hpp"
#include "../../../NXtpandedLib/src/NCircle.hpp"
#include "../../../NXtpandedLib/src/NTimer.hpp"

extern "C" {
// startup/shutdown hooks
void ecrobot_device_initialize(void);
void ecrobot_device_terminate(void);

// nxtOSEK hook to be invoked from an ISR in category 2
void user_1ms_isr_type2(void){}


using namespace nxpl;

TASK(TaskMain)
{
	NLcd lcd;

	NRectangleFilled rectangle(lcd, NPixelBox(NPoint(25, 16), 30, 30));
	NPoint m(rectangle.geometry().base().x() + rectangle.geometry().width()/2,
			 rectangle.geometry().base().y() + rectangle.geometry().height()/2);
	NCircleFilled innerC(lcd, m, rectangle.geometry().width()/2);

	NPoint diagP(NPoint(rectangle.geometry().base().x() + rectangle.geometry().width(),
			     rectangle.geometry().base().y() +rectangle.geometry().height()));
	NLine diag1(lcd, rectangle.geometry().base(), diagP);

	NTimer timer;
	bool flip = true;
	while(true) {
		if(flip) {
			NPixelBox newBox = rectangle.geometry();
			newBox.base().setX(25);
			rectangle.setGeometry(newBox);

			rectangle.show();
			innerC.hide();
			diag1.show(true);
		} else {
			NPixelBox newBox = rectangle.geometry();
			newBox.base().setX(65);
			rectangle.setGeometry(newBox);

			rectangle.hide();
			innerC.show();
			diag1.show(true);
		}

		flip = !flip;
		timer.wait(1000);
	}
 	TerminateTask();
}

}
