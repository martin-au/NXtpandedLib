/* sample.cpp for TOPPERS/ATK(OSEK) */ 

// fix this paths
extern "C" {
#include "../../../toppers_osek/include/kernel.h"
#include "kernel_id.h"
#include "../../../ecrobot/c/ecrobot_interface.h"
}

// C++ Includes and objects should be defined here.
// fix this paths also
#include "../../../NXtpandedLib/src/NLabel.hpp"

extern "C" {

void user_1ms_isr_type2(void) {
}

using namespace nxpl;

TASK(TaskMain)
{
	NCursor cursor(4, 1);
	NTextBox box(cursor, 10, 1);

	NLabel label1(cursor.asString(), box);

	cursor.moveNextLine();
	box.setBase(cursor);

	// get the cursor directly from the box
	NLabel label2(box.base().asString(), box);

	label1.show();
	label2.show(true);


	systick_wait_ms(10000);

	// Shutdown Program
	StatusType ercd = E_OK;
	ShutdownOS(ercd);
}

}

