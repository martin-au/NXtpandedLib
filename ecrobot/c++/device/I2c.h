//
// I2c.h
//
// Copyright 2009 by Takashi Chikamasa, Jon C. Martin and Robert W. Kramer
//

#ifndef I2C_H_
#define I2C_H_

#include "Port.h"

extern "C"
{
	#include "ecrobot_interface.h"
	#include "rtoscalls.h"
};

namespace ecrobot
{
/**
 * I2C device class.
 *
 * [ How to use with TOPPERS/ATK1(OSEK) ]<BR>
 * I2C (derived/compositted) class and Clock class internally use RTOS features.
 * Thus, user needs to implement a C function and RTOS provided events.<BR><BR>
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
class I2c
{
public:
	/**
	 * Constructor (activate I2C).
	 * Note:<BR>
	 * This class must be constructed as a global object. Otherwise, a device assertion will be displayed<BR>
	 * in the LCD when the object is constructed as a non global object.<BR>
	 * @param port I2C device connected port
	 * @param power Power mode
	 * @return -
	 */
	explicit I2c(ePortS port, ePower power=POWER_LOWSPEED);

	/**
	 * Destructor (de-activate I2C).
	 * @param -
	 * @return -
	 */
	virtual ~I2c(void);

	/**
	 * Send data.
	 * @param regAddr I2C address
	 * @param data Data to be sent
	 * @param length Length of data to be sent
	 * @return The result of send data: true(succeded)/false(failed)
	 */
	bool send(U32 regAddr, U8* data, U32 length);

	/***/
	// Writes a byte to a given register of the I2C device
	bool sendByte(U8 regAddr, U8 data) {
	  return send(regAddr, &data, 1);
	}
	
	bool send(U32 address, SINT regAddr, U8* data, U32 length);

	bool receive(U32 address, SINT regAddr, U8* data, U32 length) const;
	
	bool send(U32 address, SINT regAddr, SINT nInternalAddressBytes, U8* data, U32 length);

	bool receive(U32 address, SINT regAddr, SINT nInternalAddressBytes, U8* data, U32 length) const;
	
	
	/**
	 * Receive data.
	 * @param regAddr I2C address
	 * @param data Data to be received
	 * @param length Length of data to be received
	 * @return The result of receive data: true(succeded)/false(failed)
	 */
	bool receive(U32 regAddr, U8* data, U32 length) const;

	/**
	 * \brief Check if i2c busy is ready to send/receive data
	 *
	 * If i2c is not ready and you call a send/receive function the actual task
	 * will go into waiting state. This is good so other task get time to run.
	 * However if you have a real time task with low execution time with a lot of i2c calls
	 * the task may not get its timing and the system gets in undefined behaviour.
	 * To prevent this you may check if i2c is ready before the call to read/write so you can either
	 * wait in the same task until rdy or you wait until next task loop.
	 *
	 * Remember also that a lot of sensors do not give you always new values if you request the value
	 * to often!
	 *
	 * @return true if ready for read/write
	 */
	bool isReady() const {return (i2c_busy(mPort) == 0);}
protected:
	/**
	 * Get the I2C device connected port.
	 * @param -
	 * @return I2C device connected port
	 */
	inline ePortS getPort(void) const { return mPort; }

private:
	ePortS mPort;
};
}

#endif
