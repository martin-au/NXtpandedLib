/* sample.cpp for TOPPERS/ATK(OSEK) */ 

extern "C" {
#include "../../../toppers_osek/include/kernel.h"
}

// C++ Includes and objects should be defined here.
#include "../../nxtpandedlib.h"

extern "C" {

// nxtOSEK hook to be invoked from an ISR in category 2
void user_1ms_isr_type2(void){}

TASK(TaskMain)
{
	NLcd lcd; // our objects should be directly on nxt lcd.

	// base rectangle (base point, width, height)
	NRectangleFilled rectangle(&lcd, NPixelBox(NPoint(25, 16), 30, 30));

	// see how everything is coupled to the base rectangle
	NPoint m(rectangle.geometry().base().x() + rectangle.geometry().width()/2,
			 rectangle.geometry().base().y() + rectangle.geometry().height()/2);

	NCircleFilled innerC(&lcd, m, rectangle.width()/2);

	NPoint diagP(NPoint(rectangle.baseX() + rectangle.width(),
						rectangle.baseY() + rectangle.height()));

	NLine diag1(&lcd, rectangle.geometry().base(), diagP);

	// now play around with visibility

	bool flip = true;
	while(true) {
		if(flip) {
			rectangle.show();
			innerC.hide();
			diag1.show(true);
		} else {
			rectangle.hide();
			innerC.show();
			diag1.show(true);
		}
		flip = !flip;
		NNxt::wait(1000);
	}
}

}
