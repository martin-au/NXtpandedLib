/* sample.cpp for TOPPERS/ATK(OSEK) */ 

// fix this paths
extern "C" {
#include "../../../toppers_osek/include/kernel.h"
}

// C++ Includes and objects should be defined here.
#include "../../../NXtpandedLib/src/NOstream.hpp"
#include "../../../NXtpandedLib/src/NString.hpp"
#include "../../../NXtpandedLib/src/NNxt.hpp"

extern "C" {

// nxtOSEK hook to be invoked from an ISR in category 2
void user_1ms_isr_type2(void){}

TASK(TaskMain)
{
	NString s1("Hello");
	NString s2(" World!");

	cout << "s1:" << s1 << '\n';
	cout << "s2:" << s2 << '\n';

	NString s3 = s1 + s2;
	cout << "s3:" << s3 << '\n';
	cout << "Size:" << s3.size() << '\n';

	NString worldOnly = s2.substr(1);
	S32 posOfWorld = s3.find(worldOnly);
	cout << "WorldPos: " << posOfWorld << '\n';

	S32 number = -2014;
	// give buffer length so we need not resize later
	NString s4(numDigits(number)+1);
	s4.append(number);
	cout << "s4: " << s4 << '\n';
	cout << "digits:" << numDigits(number) << '\n';
	cout << "first:" << s4.at(0);

	cout.flush(true);

	NNxt::wait(10000);
	NNxt::restart();
}

}

