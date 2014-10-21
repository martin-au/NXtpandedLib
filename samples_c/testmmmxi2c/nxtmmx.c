/* nxtmmx.c */
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"

#define MMX_I2C_ADDR 			0x03
#define NXT_PORT  				NXT_PORT_S1

U32 count = 0;
/* LEJOS OSEK hooks */
void ecrobot_device_initialize()
{
	nxt_avr_set_input_power(NXT_PORT, LOWSPEED_9V);
	i2c_enable(NXT_PORT, I2C_LEGO_MODE);//I2C_LEGO_MODE);
}

void ecrobot_device_terminate()
{
	i2c_disable(NXT_PORT);
}

void user_1ms_isr_type2(void){}

/* TaskControl executed every 10msec */
TASK(OSEK_Task_Background)
{
    S32 count = 0;

	while(1)
	{
		int status = i2c_busy(NXT_PORT);
		while(status != 0)
		{
			systick_wait_ms(11); /* 10msec wait */
			display_goto_xy(0, 2);
			display_int(status, 0);
			display_update();
			status = i2c_busy(NXT_PORT);
		}
	
		U8 data[4] = {0, 0, 0, 0};

		U8 senddata[1] = {0x62};
		
		int sendstat = i2c_start(NXT_
		PORT, MMX_I2C_ADDR, senddata, 1, 4);
		
		display_goto_xy(0, 0);
		display_int(sendstat, 0);
		
		systick_wait_ms(100); /* 100ms wait */
		int completestat = i2c_complete(NXT_PORT, data, 4);
		count = count | (U32)data[0] | ((U32)data[1]) << 8 | ((U32)data[2]) << 16 | ((U32)data[3]) << 24;
		
		display_goto_xy(0, 1);
		display_int(completestat, 0);
		
		display_goto_xy(0, 3);
		display_int(count, 0);
		display_update();
		systick_wait_ms(100); /* 10msec wait */
	}
	systick_wait_ms(100); /* 10msec wait */
  	TerminateTask();
}

