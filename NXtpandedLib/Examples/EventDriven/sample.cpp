/* sample.cpp for TOPPERS/ATK(OSEK) */ 

// fix this paths
extern "C" {
#include "../../../toppers_osek/include/kernel.h"
}

DeclareEvent(PressButtonEvent);
DeclareTask(EventDispatcher);
DeclareTask(EventCallback);

// C++ Includes and objects should be defined here.
#include "../../../NXtpandedLib/src/NOstream.hpp"
#include "../../../NXtpandedLib/nxtpandedlib.h"

extern "C" {

// nxtOSEK hook to be invoked from an ISR in category 2
void user_1ms_isr_type2(void){}

// Press orange button and EventCallback task gets executed
TASK(EventDispatcher)
{
  while(1) {
	  if(NNxt::isOrangeRectPressed()) {
		  SetEvent(EventCallback, PressButtonEvent);
	  }
	  NNxt::wait(200);

  }
  TerminateTask();
}

/* EventCallback executed by OSEK Events */
TASK(EventCallback)
{
  while(1)
  {
    WaitEvent(PressButtonEvent);
    ClearEvent(PressButtonEvent);

    cout << "Pressed Button" << endl;
    NLcd::update();
  }
  TerminateTask();
}


}

