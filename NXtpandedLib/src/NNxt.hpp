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
	eButton getButtonsState(void) const
	{
		return static_cast<eButton>(ecrobot_get_button_state());
	}

	/** \brief True if orange colored rectangle button is pressed */
	bool isOrangeRectPressed()
	{
		return (bool) getButtonsState() & ORANGE_RECT;
	}

	/** \brief True if gray colored rectangle button is pressed */
	bool isGrayRectPressed()
	{
		return (bool) getButtonsState() & GRAY_RECT;
	}

	/** \brief True if left button button is pressed */
	bool isLeftPressed()
	{
		return (bool) getButtonsState() & LEFT;
	}

	/** \brief True if right button button is pressed */
	bool isRightPressed()
	{
		return (bool) getButtonsState() & RIGHT;
	}

	/**
	 * Get battery voltage in mV.
	 * @param -
	 * @return Battery voltage in mV
	 */
	S16 getBatteryVoltage(void) const
	{
		return static_cast<S16>(ecrobot_get_battery_voltage());
	}

	/**
	 * Shutdown (Power off) the NXT
	 * @param -
	 * @return -
	 */
	void shutdown(void)
	{
		ecrobot_shutdown_NXT();
	}

	/**
	 * Restart the running application
	 * @param -
	 * @return -
	 */
	void restart(void)
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
	 void execNXTBIOS(void)
	 {
		 ecrobot_exec_NXT_BIOS();
	 }

private:
	 static NNxt *nxtInstance;

	/* disable construction/copy */
	NNxt(void) {}
	NNxt(const NNxt&) {}
	// NNxt & operator = (const NNxt& n) {n;}
};

NNxt* NNxt::nxtInstance = 0;

#endif /* _NNXT_HPP_ */
