/* sample.cpp for TOPPERS/ATK(OSEK) */ 

extern "C" {
#include "../../../toppers_osek/include/kernel.h"
}

// C++ Includes and objects should be defined here.
// fix this paths also
#include "../../nxtpandedlib.h"

extern "C" {

void user_1ms_isr_type2(void) {}

TASK(TaskMain)
{
	// (indent, line)
	NCursor cursor(4, 1);
	// (cursor, max characters, lines)
	NTextBox box(cursor, 10, 1);

	// create label and set the text to cursor position string.
	NLabel label1(cursor.asString(), box);

	// now create label with same box in next line.
	cursor.moveNextLine();
	box.setBase(cursor);

	// get the cursor directly from the box
	NLabel label2(box.base().asString(), box);

	label1.show();
	label2.show(true);

	NNxt::wait(10000);
	NNxt::restart();
}

}

