/* sample.cpp for TOPPERS/ATK(OSEK) */ 

// fix this paths
extern "C" {
#include "../../../toppers_osek/include/kernel.h"
}

DeclareResource(ConsoleWidgetResource);

// C++ Includes and objects should be defined here.
#include "../../nxtpandedlib.h"
#include "../../src/NLcdView.hpp"

NMutex consoleMutext(ConsoleWidgetResource);

extern "C" {

// nxtOSEK hook to be invoked from an ISR in category 2
void user_1ms_isr_type2(void){}

TASK(TaskMain)
{
	NLabel toRight(">>", NTextBox(NCursor(14, 3), 2, 1));
	NLabel toLeft("<<", NTextBox(NCursor(0, 3), 2, 1));
	NLabel lastSite("(main)", NTextBox(NCursor(5, 3), 7, 1));

	NConsole console(consoleMutext, NTextBox(NCursor(3, 0), 14, 8));

	NLcd lcd;
	NRectangleFilled rectangle(&lcd, NPixelBox(NPoint(20, 20), 30, 30));

	// now create different views
	NLcdView mainSite(3);
	mainSite.add(&toRight);
	mainSite.add(&toLeft);
	mainSite.add(&lastSite);

	NLcdView rightSite(2);
	rightSite.add(&toLeft);
	rightSite.add(&console);

	NLcdView leftSite(2);
	leftSite.add(&toRight);
	leftSite.add(&rectangle);

	const S8 mainSiteId = 0;
	const S8 leftSiteId = -1;
	const S8 rightSiteId = 1;

	mainSite.show(true);
	int site = mainSiteId;


	while(true) {
		switch(site) {
		case leftSiteId:
			if(NNxt::isRightPressed()) {
				NLcdView::swap(leftSite, mainSite, true);
				site = mainSiteId;

				console << "right pressed\n";
				lastSite.setText("Left");
			}
			break;
		case mainSiteId:
			if (NNxt::isRightPressed()) {
				NLcdView::swap(mainSite, rightSite, true);
				site = rightSiteId;
				console << "right pressed\n";
			} else if(NNxt::isLeftPressed()) {
				NLcdView::swap(mainSite, leftSite, true);
				site = leftSiteId;
				console << "left pressed\n";
			}

			break;
		case rightSiteId:
			if (NNxt::isLeftPressed()) {
				NLcdView::swap(rightSite, mainSite, true);
				site = mainSiteId;

				console << "left pressed\n";
				lastSite.setText("Right");
			}
			break;
		}

		if(NNxt::isGrayRectPressed())
			NNxt::shutdown();

		NNxt::wait(300);
	}


	NNxt::wait(10000);
	TerminateTask();
}

}

