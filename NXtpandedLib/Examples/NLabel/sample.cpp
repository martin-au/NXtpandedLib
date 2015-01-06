/* sample.cpp for TOPPERS/ATK(OSEK) */ 


extern "C" {
#include "../../../toppers_osek/include/kernel.h"
}

// C++ Includes and objects should be defined here.
#include "../../../NXtpandedLib/src/NLabel.hpp"
#include "../../../NXtpandedLib/src/NNxt.hpp"

extern "C" {

void user_1ms_isr_type2(void) {}

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
	NNxt::wait(4000);

	// change properties of label2 and set float number
	label2.setPrecision(3);
	label2.setLine(5);
	label2.setNumber(12.12345f);

	label1.hide();
	label2.show(true);

	NNxt::wait(10000);
	NNxt::restart();
}

}

