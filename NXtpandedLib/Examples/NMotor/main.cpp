/* sample.cpp for TOPPERS/ATK(OSEK) */ 

extern "C" {
#include "../../../toppers_osek/include/kernel.h"
#include "kernel_id.h"
}


// DeclareCounter(SysTimerCnt);
// DeclareResource(motorResource);

#include "../../src/NTimer.hpp"
#include "../../src/Mutex.hpp"

using namespace nxpl;

// mutex_t motorMtx(motorResource);

extern "C" {
// nxtOSEK hook to be invoked from an ISR in category 2
void user_1ms_isr_type2(void) {
	/*
	SleeperMonitor();
	StatusType ercd;

	ercd = SignalCounter(SysTimerCnt); // Increment OSEK Alarm Counter
	if (ercd != E_OK) {
	    ShutdownOS(ercd);
	}
	*/
}
}

#include "../../src/NMotor.hpp"
#include "../../src/NMMXMotor.hpp"

NTimer timer;

//NMotor motorAInst(PORT_A); // nxt motor instance
//NMMXMotor motorAInst(PORT_1, MMXPortM1);
//NIMotor *motorA = &motorAInst; // interface abstraction of motor

NMotor motorBInst(PORT_B); // nxt motor instance
NIMotor *motorB = &motorBInst; // interface abstraction of motor


#include "../../src/NLabel.hpp"
#include "../../src/NPairBox.hpp"

NLabel indicatorTextCount("Count:", NTextBox(NCursor(), 8));
NLabel indicatorCount(NTextBox(NCursor(), 7));
NPairBox pairBoxCount(&indicatorTextCount, &indicatorCount, NCursor(0, 0), NAlignment::right());

NLabel indicatorTextPower("Power:", NTextBox(NCursor(), 8));
NLabel indicatorPower(NTextBox(NCursor(), 7));
NPairBox pairBoxPower(&indicatorTextPower, &indicatorPower, NCursor(0, 1), NAlignment::right());


NLabel indicatorTextStatus("Status:", NTextBox(NCursor(), 8));
NLabel indicatorStatus(NTextBox(NCursor(), 7));
NPairBox pairBoxStatus(&indicatorTextStatus, &indicatorStatus, NCursor(0, 2), NAlignment::right());



extern "C" {

void ecrobot_device_initialize()
{
	ecrobot_MMX_init(NXT_PORT_S1);
}

void ecrobot_device_terminate()
{
	ecrobot_term_MMX(NXT_PORT_S1);
}


S32 count = 0;
S32 lastCount = -9999;
int cnt = 0;
TASK(TaskDisplay)
{

    while(1) {

		if (ecrobot_is_RUN_button_pressed())
		{

			// ecrobot_set_MMX_time(NXT_PORT_S1, 0x03, MMX_Motor_1, 50, 3, 1);
			display_clear(0);
		}

    	// motorMtx.acquire();

    	lastCount = count;

    	U8 data[4] = {0};

    	// count = ecrobot_get_MMX_count(NXT_PORT_S1, 0x03, MMX_Motor_1);

    	//indicatorStatus.setNumber(count);

    	ecrobot_wait_i2c_ready(NXT_PORT_S1, 50);
    	i2c_start_transaction(NXT_PORT_S1, 0x03, 0x62, 1, data, 4, 0);
    	// indicatorStatus.setNumber(0);

    	count = count | (U32)data[0] | ((U32)data[1]) << 8 | ((U32)data[2]) << 16 | ((U32)data[3]) << 24;

    	indicatorCount.setNumber(lastCount);
    	// while (i2c_busy(NXT_PORT_S1) != 0);

    	//indicatorPower.setNumber(motorA->getPower());
    	// indicatorPower.setNumber(cnt++);
    	/*
		NString status(7);
		if(motorA->isMoving()) {
			status = "moving";
		} else if(motorA->isFloating()) {
			status = "floating";
		} else if(motorA->isStopped()) {
			status = "stopped";
		}
    	 */

    	// indicatorStatus.setNumber(motorAInst.getStatus());
    	// motorMtx.release();

    	pairBoxCount.show(true);
    	// pairBoxPower.show();
    	//pairBoxStatus.show(true);

    	// systick_wait_ms(200);
    }


	TerminateTask();
}

}

