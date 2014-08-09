/* sample.cpp for TOPPERS/ATK(OSEK) */ 

// fix this paths
extern "C" {
#include "../../../toppers_osek/include/kernel.h"
#include "kernel_id.h"
#include "../../../ecrobot/c/ecrobot_interface.h"
}


DeclareCounter(SysTimerCnt);
DeclareResource(OstreamResource);
DeclareEvent(PressButtonEvent);
DeclareTask(EventDispatcher);
DeclareTask(EventCallback);

// C++ Includes and objects should be defined here.
// fix this paths also
#include "../../../NXtpandedLib/src/NOstream.hpp"
#include "../../../NXtpandedLib/src/NTimer.hpp"

#include "../../../ecrobot/C++/device/Nxt.h";

nxpl::mutex_t ostreamMtx(OstreamResource);
nxpl::NOstream cout(ostreamMtx);
nxpl::NTimer timer;

extern "C" {
// startup/shutdown hooks
void ecrobot_device_initialize(void);
void ecrobot_device_terminate(void);

// nxtOSEK hook to be invoked from an ISR in category 2
void user_1ms_isr_type2(void){
	SleeperMonitor();
	StatusType ercd;

	ercd = SignalCounter(SysTimerCnt); /* Increment OSEK Alarm Counter */
	if (ercd != E_OK) {
	    ShutdownOS(ercd);
	}
}


TASK(EventDispatcher)
{
  ecrobot::Nxt nxt;
  while(1) {
	  if(ecrobot::Nxt::ORANGE_RECT == nxt.getNxtButtons()) {
		  SetEvent(EventCallback, PressButtonEvent);
	  }
	  timer.sleep(200);

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

    cout << "Pressed Button" << nxpl::endl;
    display_update();
  }
  TerminateTask();
}


}

