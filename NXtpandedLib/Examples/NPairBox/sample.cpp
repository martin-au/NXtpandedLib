/* sample.cpp for TOPPERS/ATK(OSEK) */ 

// fix this paths
extern "C" {
#include "../../../toppers_osek/include/kernel.h"
#include "kernel_id.h"
#include "../../../ecrobot/c/ecrobot_interface.h"
}

DeclareCounter(SysTimerCnt);
DeclareResource(OstreamResource);

// C++ Includes and objects should be defined here.
// fix this paths also
#include "../../../NXtpandedLib/src/NOstream.hpp"
#include "../../../NXtpandedLib/src/NLabel.hpp"
#include "../../../NXtpandedLib/src/NPairBox.hpp"
#include "../../../NXtpandedLib/src/NTimer.hpp"

nxpl::NMutex ostreamMtx(OstreamResource);
nxpl::NOstream cout(ostreamMtx);

extern "C" {
// startup/shutdown hooks
void ecrobot_device_initialize(void);
void ecrobot_device_terminate(void);

// nxtOSEK hook to be invoked from an ISR in category 2
void user_1ms_isr_type2(void){
	StatusType ercd;

	ercd = SignalCounter(SysTimerCnt); /* Increment OSEK Alarm Counter */
	if (ercd != E_OK) {
	    ShutdownOS(ercd);
	}
}


using namespace nxpl;

TASK(TaskMain)
{
	// A number indicator and the text label which shows the name of the number
	NLabel indicator(NTextBox(NCursor(8, 5), 1));
	NLabel indicatorText("Value:", NTextBox(NCursor(0, 0), 6));

	// pair them together, first parameter is the main widget
	// the main widget specifies the position of the pair
	NPairBox pairBox1(&indicator, &indicatorText);

	// now show all align options for the second widget
	NTimer timer;
	U8 i = 0;
	while(true) {
		switch (i) {
		case 0:
			pairBox1.align2Main(NAlignment::top());
			break;
		case 1:
			pairBox1.align2Main(NAlignment::right());
			break;
		case 2:
			pairBox1.align2Main(NAlignment::bottom());
			break;
		case 3:
			pairBox1.align2Main(NAlignment::left());
			break;
		}
		indicator.setNumber(i);
		pairBox1.show(true);

		++i;
		if(i > 3) i = 0;
		timer.wait(1000);
	}

	TerminateTask();
}

}

