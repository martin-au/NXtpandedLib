/* sample.cpp for TOPPERS/ATK(OSEK) */ 

// fix this paths
extern "C" {
#include "../../../toppers_osek/include/kernel.h"
}

// C++ Includes and objects should be defined here.
#include "../../../NXtpandedLib/nxtpandedlib.h"

extern "C" {

// nxtOSEK hook to be invoked from an ISR in category 2
void user_1ms_isr_type2(void){}

TASK(TaskMain)
{
	// A number indicator and the text label which shows the name of the number
	NLabel indicator(NTextBox(NCursor(8, 5), 1));
	NLabel indicatorText("Value:", NTextBox(NCursor(0, 0), 6));

	// pair them together, first parameter is the main widget
	// the main widget specifies the position of the pair
	NPairBox pairBox1(&indicator, &indicatorText);

	// now show all align options for the second widget
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
		NNxt::wait(1000);
	}

	TerminateTask();
}

}

