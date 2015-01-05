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
	int myIntNumber = 123;
	float myFloatNumber = 32.12345;

	cout << "myIntNum: " << myIntNumber << '\n';
	cout << "as Hex: " << hex << myIntNumber << "\n";

	cout << "---------------\n";

	cout << "Float: " << myFloatNumber << '\n';
    cout << "Precision: " <<  cout.precision() << "\n";

    cout.precision(4);
    cout << "Float: " << myFloatNumber << '\n';
    cout << "Precision: " <<  cout.precision() << "\n";

	cout.flush(true);

	NNxt::wait(5000);
	NNxt::restart();
}

}

