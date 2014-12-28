/* sample.cpp for TOPPERS/ATK(OSEK) */ 

// fix this paths
extern "C" {
#include "../../../toppers_osek/include/kernel.h"
#include "kernel_id.h"
#include "../../../ecrobot/c/ecrobot_interface.h"
}


DeclareCounter(SysTimerCnt);
DeclareResource(OstreamResource);

#include "../../../ecrobot/c++/device/nxt.h" // buttons
// C++ Includes and objects should be defined here.
// fix this paths also
#include "../../../NXtpandedLib/src/NOstream.hpp"
#include "../../../NXtpandedLib/src/NTimer.hpp"
#include "../../../NXtpandedLib/src/NRectangle.hpp"
#include "../../../NXtpandedLib/src/NLabel.hpp"
#include "../../../NXtpandedLib/src/NLcdView.hpp"

nxpl::NMutex ostreamMtx(OstreamResource);

extern "C" {
// startup/shutdown hooks
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


using namespace nxpl;

TASK(TaskMain)
{
	NLabel test("(test)", NTextBox(NCursor(5, 0), 7, 1));
	test.show(true);

	NLabel toRight(">>", NTextBox(NCursor(14, 3), 2, 1));
	NLabel toLeft("<<", NTextBox(NCursor(0, 3), 2, 1));
	NLabel lastSite("(main)", NTextBox(NCursor(5, 3), 7, 1));

	NOstream cout(ostreamMtx, NTextBox(NCursor(3, 0), 14, 8));

	NLcd lcd;
	NRectangleFilled rectangle(lcd, NPixelBox(NPoint(20, 20), 30, 30));

	NLcdView mainSite(4);
	mainSite.add(&toRight);
	mainSite.add(&toLeft);
	mainSite.add(&lastSite);

	NLcdView rightSite(3);
	rightSite.add(&toLeft);
	rightSite.add(&cout);

	NLcdView leftSite(3);
	leftSite.add(&toRight);
	leftSite.add(&rectangle);

	NTimer timer;

	const S8 mainSiteId = 0;
	const S8 leftSiteId = -1;
	const S8 rightSiteId = 1;

	mainSite.show(true);
	test.show(true);
	int site = mainSiteId;

	ecrobot::Nxt nxt;

	while(true) {
		ecrobot::Nxt::eNxtButton but = nxt.getNxtButtons();
		switch(site) {
		case leftSiteId:
			if(but & ecrobot::Nxt::RIGHT) {
				NLcdView::swap(leftSite, mainSite, true);
				site = mainSiteId;

				cout << "right pressed\n";
				lastSite.setText("Left");
			}
			break;
		case mainSiteId:
			/*
			if (but & ecrobot::Nxt::RIGHT) {
				NLcdView::swap(mainSite, rightSite, true);
				site = rightSiteId;
				cout << "right pressed\n";
			} else if(but & ecrobot::Nxt::LEFT) {
				NLcdView::swap(mainSite, leftSite, true);
				site = leftSiteId;
				cout << "left pressed\n";
			}
			*/
			break;
		case rightSiteId:
			if (but & ecrobot::Nxt::LEFT) {
				NLcdView::swap(rightSite, mainSite, true);
				site = mainSiteId;

				cout << "left pressed\n";
				lastSite.setText("Right");
			}
			break;
		}
		timer.wait(300);

	}


	TerminateTask();
}

}

