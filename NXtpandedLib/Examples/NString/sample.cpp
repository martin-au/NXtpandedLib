/* sample.cpp for TOPPERS/ATK(OSEK) */ 

// fix this paths
extern "C" {
#include "../../../toppers_osek/include/kernel.h"
#include "kernel_id.h"
#include "../../../ecrobot/c/ecrobot_interface.h"
}

DeclareResource(OstreamResource);

// C++ Includes and objects should be defined here.
// fix this paths also
#include "../../../NXtpandedLib/src/NOstream.hpp"
#include "../../../NXtpandedLib/src/NString.hpp"

nxpl::NMutex ostreamMtx(OstreamResource);
nxpl::NOstream cout(ostreamMtx);

extern "C" {
// startup/shutdown hooks
void ecrobot_device_initialize(void);
void ecrobot_device_terminate(void);

// nxtOSEK hook to be invoked from an ISR in category 2
void user_1ms_isr_type2(void){
}

TASK(TaskMain)
{
	nxpl::NString s1("Hello");
	nxpl::NString s2(" World!");

	cout << "s1: " << s1 << '\n';
	cout << "s2: " << s2 << '\n';

	nxpl::NString s3 = s1 + s2;
	cout << "s3: " << s3 << '\n';
	cout << "Size:" << s3.size() << '\n';

	nxpl::NString worldOnly = s2.substr(1);
	S32 posOfWorld = s3.find(worldOnly);
	cout << "WorldPos: " << posOfWorld << '\n';

	S32 number = -2014;
	// give buffer length so we need not resize later
	nxpl::NString s4(nxpl::numDigits(number)+1);
	s4.append(number);
	cout << "s4: " << s4 << '\n';
	cout << "digits:" << nxpl::numDigits(number) << '\n';
	cout << "first:" << s4.at(0);

	cout.flush(true);
	TerminateTask();
}

}

