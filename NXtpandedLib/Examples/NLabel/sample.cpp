/* sample.cpp for TOPPERS/ATK(OSEK) */ 

// fix this paths
extern "C" {
#include "../../../toppers_osek/include/kernel.h"
#include "kernel_id.h"
#include "../../../ecrobot/c/ecrobot_interface.h"
#
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
	// text box with a position (4 indent, first line, max chars in line 10)
	NTextBox baseBox(NCursor(4, 1), 10);

	// create a const label with const position and const text
	const NLabel label1("MyProgram", baseBox);

	// move the text box to line 3 and keep other parameters
	baseBox.setLine(3);

	// create new label with no text
	NLabel label2(baseBox);

	label2.setNumber(1234);

	// make the labels visible on next display update, show(true) will finally update the display
	label1.show();
	label2.show(true);

	// wait 4 seconds
	systick_wait_ms(4000);

	label2.setPrecision(3);
	label2.setLine(5);
	label2.setNumber(12.12345f);

	label1.hide();
	label2.show(true);

	systick_wait_ms(10000);

	// Shutdown Program
	StatusType ercd = E_OK;
	ShutdownOS(ercd);
}

}

