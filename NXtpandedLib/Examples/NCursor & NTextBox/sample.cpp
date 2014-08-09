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

// for code correction
#include "../../../NXtpandedLib/src/NTextBox.hpp"
#include "../../../NXtpandedLib/src/NCursor.hpp"

nxpl::mutex_t ostreamMtx(OstreamResource);
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
	NCursor cursor(4, 1);
	NTextBox box(cursor, 10, 1);

	NLabel label1(cursor.asString(), box);

	cursor.moveNextLine();
	box.setBase(cursor);

	NLabel label2(box.base().asString(), box);

	label1.show();
	label2.show(true);

	TerminateTask();
}

}

