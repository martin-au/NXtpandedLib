/* sample.cpp for TOPPERS/ATK(OSEK) */ 

// ECRobot++ API
#include "Clock.h"
#include "Lcd.h"
using namespace ecrobot;

extern "C"
{
#include "kernel.h"
#include "kernel_id.h"
#include "com_cfg.h"
#include "ecrobot_interface.h"
#include <stdio.h>

/* nxtOSEK hook to be invoked from an ISR in category 2 */
void user_1ms_isr_type2(void)
{
	SleeperMonitor();
}

Clock clk;
Lcd lcd;
MSG1 msg1, msg2;

TASK(OSEK_Task_1)
{
	lcd.clear();

	StartCOM(COMAPP);

	sprintf(msg1.buf,"Msg1\n");
	SendMessage(SendMsg,&msg1);

	clk.sleep(500);

	sprintf(msg1.buf,"Msg2\n");
	SendMessage(SendMsg2,&msg1);

	TerminateTask();
}

TASK(OSEK_Task_2)
{
	RecieveMessage(RcvMsg,&msg2);

	lcd.putf("s", msg2.buf);
	lcd.disp();

	TerminateTask();
}

TASK(OSEK_Task_3)
{
	RecieveMessage(RcvMsg2,&msg2);

	lcd.putf("s", msg2.buf);
	lcd.disp();

	while(1);
}
}
