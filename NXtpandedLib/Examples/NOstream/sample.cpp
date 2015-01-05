/* sample.cpp for TOPPERS/ATK(OSEK) */ 

extern "C" {
#include "../../../toppers_osek/include/kernel.h"
}

// C++ Includes and objects should be defined here.
#include "../../../NXtpandedLib/src/NOstream.hpp"
#include "../../../NXtpandedLib/src/NNxt.hpp"

extern "C" {

// nxtOSEK 1ms interrupt
void user_1ms_isr_type2(void){}

TASK(TaskMain)
{
    cout << "Hello world 1" << endl;
	cout << "Hello world 2\n";
	cout << "Hello world 3" << "\n";
	cout.flush(true);

	NNxt::wait(5000);
	NNxt::restart();
}

}

