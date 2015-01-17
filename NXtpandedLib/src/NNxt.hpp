/*
 * NNxt.hpp
 *
 *  Created on: 02.01.2015
 *      Author: Martin
 */

#ifndef _NNXT_HPP_
#define _NNXT_HPP_

extern "C" {
#include "../../ecrobot/c/ecrobot_interface.h"
#include "../../ecrobot/c/rtoscalls.h"
}

/**
 * NXT intelligent block class.
 *
 * Use NNxt::instance() to get the nxt object.
 * Example: NNxt::instance()->restart();
 */
class NNxt
{
public:

	/* Singleton not essential now.
	static NNxt* instance()
    {
		if(!nxtInstance)
			nxtInstance = new NNxt();
		return nxtInstance;
    }

    ~NNxt() {
    	delete nxtInstance;
    	nxtInstance = 0;
    }
    */

	/**
	 * NXT Button enum.
	 * Note that left triangle and gray colored rectangle buttons on the NXT are reserved for stopping and shutdown application program.
	 * To use these buttons for an application, define NO_RUN_ENTER_STOP_EXIT compile switch macro while compiling the application.
	 */
	enum eButton
	{
		ORANGE_RECT = 0x01,               /**< Orange colored rectangle button */
		LEFT = 0x02,                      /**< Left triangle button            */
		RIGHT = 0x04,                     /**< Right triangle button           */
		GRAY_RECT = 0x08,                 /**< Gray colored rectangle button   */
	};

	/**
	 * Get NXT buttons status.
	 * @param -
	 * @return Status of all buttons on the NXT (true:pressed/false:not pressed)
	 */
	static eButton getButtonsState(void)
	{
		return static_cast<eButton>(ecrobot_get_button_state());
	}

	/** \brief True if any button pressed */
	static bool isAnyButtonPressed()
	{
		return getButtonsState() > 0;
	}

	/** \brief True if orange colored rectangle button is pressed */
	static bool isOrangeRectPressed()
	{
		return getButtonsState() == ORANGE_RECT;
	}

	/** \brief True if gray colored rectangle button is pressed */
	static bool isGrayRectPressed()
	{
		return getButtonsState() == GRAY_RECT;
	}

	/** \brief True if left button button is pressed */
	static bool isLeftPressed()
	{
		return getButtonsState() == LEFT;
	}

	/** \brief True if right button button is pressed */
	static bool isRightPressed()
	{
		return getButtonsState() == RIGHT;
	}

	/**
	 * Get battery voltage in mV.
	 * @param -
	 * @return Battery voltage in mV
	 */
	static S16 getBatteryVoltage(void)
	{
		return static_cast<S16>(ecrobot_get_battery_voltage());
	}

	/**
	 * Shutdown (Power off) the NXT
	 * @param -
	 * @return -
	 */
	static void shutdown(void)
	{
		ecrobot_shutdown_NXT();
	}

	/**
	 * Restart the running application
	 * @param -
	 * @return -
	 */
	static void restart(void)
	{
		ecrobot_restart_NXT();
	}

	/**
	 * Execute NXT BIOS.
	 * Note that this API works only when NXT BIOS is used. Otherwise, it does nothing.<BR>
	 * Note that NXT BIOS 1.0.4 or later is required.
	 * @param -
	 * @return -
	 */
	 static void execNXTBIOS(void)
	 {
		 ecrobot_exec_NXT_BIOS();
	 }

	 /**
	  * \brief Gets system tick in msec.
	  *
	  * System tick is started when the NXT is turned on (not started when an application begins)
	  */
	 static U32 getTick()
	 {
		 return systick_get_ms();
	 }

	 /**
	  * \brief Wait in a loop for duration in msec.
	  */
	 static void wait(U32 ms)
	 {
		 systick_wait_ms(ms);
	 }

	 /**
	  * Let the task sleep for ms duration. Lets lower priority tasks execute.
	  * User needs to implement a C function and RTOS provided events.
	  *
	  * User needs to implement a C function and RTOS provided events.<BR><BR>
	  * + Invoke SleeperMonitor function in user_1msec_isr_type2 hook.<BR>
	  * &nbsp;&nbsp;void user_1ms_isr_type2(void)<BR>
	  * &nbsp;&nbsp;{<BR>
	  * &nbsp;&nbsp;&nbsp;&nbsp;SleeperMonitor(); // needed for I2C devices and Clock class<BR>
	  * &nbsp;&nbsp;}<BR>
	  * <BR>
	  * + Define EventSleepI2C and EventSleep Events in user oil file.<BR>
	  * &nbsp;&nbsp;EVENT EventSleepI2C<BR>
	  * &nbsp;&nbsp;{<BR>
	  * &nbsp;&nbsp;&nbsp;&nbsp;MASK = AUTO;<BR>
	  * &nbsp;&nbsp;};<BR><BR>
	  * &nbsp;&nbsp;EVENT EventSleep<BR>
	  * &nbsp;&nbsp;{<BR>
	  * &nbsp;&nbsp;&nbsp;&nbsp;MASK = AUTO;<BR>
	  * &nbsp;&nbsp;};<BR><BR>
	  * &nbsp;&nbsp;TASK TaskMain<BR>
	  * &nbsp;&nbsp;{<BR>
	  * &nbsp;&nbsp;&nbsp;&nbsp;AUTOSTART = TRUE<BR>
	  * &nbsp;&nbsp;&nbsp;&nbsp;{<BR>
	  * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;APPMODE = appmode1;<BR>
	  * &nbsp;&nbsp;&nbsp;&nbsp;};<BR>
	  * &nbsp;&nbsp;&nbsp;&nbsp;PRIORITY = 1;<BR>
	  * &nbsp;&nbsp;&nbsp;&nbsp;ACTIVATION = 1;<BR>
	  * &nbsp;&nbsp;&nbsp;&nbsp;SCHEDULE = FULL;<BR>
	  * &nbsp;&nbsp;&nbsp;&nbsp;STACKSIZE = 512;<BR>
	  * &nbsp;&nbsp;&nbsp;&nbsp;EVENT = EventSleepI2C; <- Here it is<BR>
	  * &nbsp;&nbsp;&nbsp;&nbsp;EVENT = EventSleep; <- Here it is<BR>
	  * &nbsp;&nbsp;};<BR>
	  * <BR>
	  * [ How to use with TOPPERS/JSP(ITRON) ]<BR>
	  * In case of using this class with TOPPERS/JSP(ITRON), RTOS specific definitions for the class are not needed.
	  */
	 static void sleep(U32 ms)
	 {
		 Sleep(ms);
	 }

private:
	// static NNxt *nxtInstance;

	/* disable construction/copy */
	NNxt(void) {}
	NNxt(const NNxt&) {}
	// NNxt & operator = (const NNxt& n) {n;}
};

// NNxt* NNxt::nxtInstance = 0;

#endif /* _NNXT_HPP_ */
