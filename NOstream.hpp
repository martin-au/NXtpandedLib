/*
 * NOstream
 *
 *  Created on: 30.09.2013
 *      Author: Martin
 */

#ifndef __OSTREAM_H_
#define __OSTREAM_H_

/** \file
 *	\ingroup NxtLcd
*/

extern "C" {
	#include "../../toppers_osek/include/kernel.h"
	#include "kernel_id.h"
	#include "../../ecrobot/c/ecrobot_interface.h"
	#include "../../lejos_nxj/src/nxtvm/platform/nxt/display.h"
}

#include "LcdConstants.hpp"

#include "NWidget.hpp"
#include "Uncopyable.hpp"
#include "Mutex.hpp"

// TODO new function show because this is also a NWidget?
namespace nxpl {

/**
 * \brief Create a console widget on the lcd which works with streams.
 *
 * This class provides a interface just like the std::ostream in standard C++
 * Its main indent is to see what your application does. You simply create a global
 * object called for example cout (like on computers) and then you can use it to make outputs
 * anywhere in your code. This class is task save so you can use it in all your tasks.
 */
class NOstream : public NWidget, private Uncopyable
{
private:
	// Actual cursor Line
	U8 cursorLine;
	mutable bool somenew;
	bool nextHex;
	U16 floatplaces;

	mutable mutex_t mutex;

	// Console buffer array
	char **textBuffer;

	// allowing manipulators (endl, hex ...)
	typedef NOstream& (*NOstreamManipulator)(NOstream&);

	void newline() {
		++cursorLine;
	}
	void newLineSpace();

	// the only way to put data into the textBuffer
	// responsible for text formatting...
	void streamhandler(const char *str);
public:
	/** \brief Construct a console widget.
	 *
	 * In order to make ostream task save it needs a resource/mutex.
	 * Please define the resource in oil file in every task where you use ostream.
	 *
	 * @param res See description.
	 * @param startLine The first line of the console. (0 - 7)
	 * @param rows The number of console lines.
	 * @param indent The indent in chars from left.
	 * @param width The width of the console in chars.
	 */
	explicit NOstream(mutex_t res,
			  U8 startLine = 0,
			  U8 rows = LCD::ROWS,
			  U8 indent = 0,
			  U8 width = LCD::LINE_WIDTH);
	virtual ~NOstream();

	/** \brief Copy data of buffer into lcd.
	 *
	 * Its recommended to use this not to often because it takes process time.
	 * A good solution is a display update task which has cycle time of about 100 - 400 ms.
	 * Remember the human eye is slow ;)
	 *
	 * @param update If true update the display.
	 */
	void flush(bool update = false) const;

	/** Make console visible.
	 *
	 * This is the same as NOstream::flush().
	 *
	 * @param update If true update the display.
	 */
	virtual void show(bool update = false) const {
		flush(update);
	}

	/** Make console invisible.
	 *
	 * @param update If true update the display.
	 */
	virtual void hide(bool update = false) const;

	/**
	 * \brief Get floating-point precision.
	 * @return Number of places.
	 */
	U16 precision() const;

	/** \brief Set floating-point precision.
	 *
	 * @param prec Number of places.
	 * @return Number of places.
	 */
	U16 precision(U16 prec);

	/** \brief Ostream manipulator: show next number as hex.
	 *
	 * @param stream
	 * @return
	 */
	friend NOstream& hex(NOstream& stream);

	/** \brief Ostream manipulator: end actual line and flush console.
	 *
	 * See NOstream::flush() on performance issue.
	 * Use the endl-ascii-char to make "a not flushing endl"
	 *
	 * @param stream
	 * @return
	 */
	friend NOstream& endl(NOstream& stream);

	NOstream& operator<<(const char* str); /**<Put C-String into stream.*/
	NOstream& operator<<(char str); /**<Put char into stream.*/
	NOstream& operator<<(S32 num); /**<Put signed 32bit number into stream.*/
	NOstream& operator<<(U32 num); /**<Put unsigned 32bit number into stream.*/

	NOstream& operator<<(int num) {return operator<<(static_cast<S32>(num));} /**<Put integer number into stream.*/
	NOstream& operator<<(S16 num) {return operator<<(static_cast<S32>(num));} /**<Put signed 16bit number into stream.*/
	NOstream& operator<<(U16 num) {return operator<<(static_cast<U32>(num));} /**<Put unsigned 32bit number into stream.*/
	NOstream& operator<<(S8 num)  {return operator<<(static_cast<S32>(num));} /**<Put signed 8bit number into stream.*/
	NOstream& operator<<(U8 num)  {return operator<<(static_cast<U32>(num));} /**<Put unsigned 16bit number into stream.*/

	NOstream& operator<<(float num); /**<Put floating-point-number into stream.*/

	NOstream& operator<<(bool b) { /**<Put boolean into stream. Converts bool into text form true/false*/
		if(b) return *this << "true";
		else  return *this << "false";
	}

	NOstream& operator<<(NOstream& stream) {return *this;}
	NOstream& operator<<(NOstreamManipulator manip); /**<Put manipulator into stream.*/
};

}

#endif /* __OSTREAM_H_ */
