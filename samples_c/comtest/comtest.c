/* helloworld.c for TOPPERS/ATK(OSEK) */ 
#include "kernel.h"
#include "kernel_id.h"
#include "com_cfg.h"
#include "ecrobot_interface.h"
#include <stdio.h>

/* nxtOSEK hook to be invoked from an ISR in category 2 */
void user_1ms_isr_type2(void){ /* do nothing */ }

MSG1	msg1, msg2;
TASK(OSEK_Task_1)
{
	StartCOM(COMAPP);
	sprintf(msg1.buf,"I'm Sending Msg1\n");
	SendMessage(SendMsg,&msg1);
	systick_wait_ms(500); /* 500msec wait */
	TerminateTask();
}

TASK(OSEK_Task_2)
{
	RecieveMessage(RcvMsg,&msg2); /* Recieving a message triggers activation of task (see comtest.oil) */
	display_clear(0);
	display_goto_xy(0, 0);
	display_string(msg2.buf);
	display_update();
	while(1);
}

