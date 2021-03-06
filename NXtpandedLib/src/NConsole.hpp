/*
 * NOstream.hpp
 *
 *  Created on: 21.03.2014
 *      Author: Martin
 */

#ifndef __NCONSOLE_HPP_
#define __NCONSOLE_HPP_

/** \file
 *	\ingroup NxtLcd
*/


#include "..\..\ecrobot\c\ecrobot_types.h"


#include "NString.hpp"
#include "NWidget.hpp"
#include "LcdConstants.hpp"
#include "Uncopyable.hpp"
#include "NMutex.hpp"

#include "../../../GNUARM/arm-elf/include/string.h" // strcpy, strlen

/**
 * \brief Create a console widget on the lcd which works with streams.
 *
 * This class provides a interface just like the std::ostream in standard C++
 * Its main indent is to see what your application does. You simply create a global
 * object called for example cout (like on computers) and then you can use it to make outputs
 * anywhere in your code. This class is task save so you can use it in all your tasks.
 *
 * For all output streams: If line gets to long for display, console will auto line feed with an indent.
 */
class NConsole: public NWidget, private Uncopyable {
private:
	static const U8 autoLineFeedIndent = 2;
	mutable bool somenew;
	bool nextHex;
	U16 floatplaces;

	mutable NMutex mutex;

	void textBoxValid();

	bool hasLinesChanged() const {
		return streamBuffer->currentRows != textBox().lines();
	}

	bool hasCharsInLineChanged() const {
		return streamBuffer->currentLen != textBox().charsInLine();
	}

	// Console buffer array
	struct StreamBuffer {
	public:
		explicit StreamBuffer(U8 len, U8 rows) :
				buffer(new char *[rows]), currentLen(len), currentRows(rows), cursorLine(0) {

			for (int i = 0; i < rows + 1; i++) {
				buffer[i] = new char[len + 1]; // +1 for '\0'
				strcpy(buffer[i], "");
			}
		}

		~StreamBuffer() {
			for (int i = 0; i < currentRows + 1; i++) {
				delete[] buffer[i];
			}
			delete[] buffer;
		}

		char **buffer;
		U8 currentLen;
		U8 currentRows;
		// Actual cursor Line
		U8 cursorLine;
	};
	StreamBuffer *streamBuffer;

	// allowing manipulators (endl, hex ...)
	typedef NConsole& (*NOstreamManipulator)(NConsole&);

	void newline() {
		++(streamBuffer->cursorLine);
	}
	void newLineSpace();

	// the only way to put data into the textBuffer
	// responsible for text formatting...
	void streamhandler(const char *str);

	virtual void showWidgetImpl() const;
	virtual void hideWidgetImpl() const;

	virtual void textBoxChangedHandler();
public:
	/** \brief Construct a console widget.
	 *
	 * In order to make ostream task save it needs a resource/mutex.
	 * You have to define the resource in oil file in every task where you use ostream.
	 * You can specify the size and position of the widget with the box argument.
	 *
	 * @param res See description.
	 * @param box TextBox in which the console should exist. Default is a TextBox over the whole nxt lcd.
	 */
	explicit NConsole(NMutex res, NTextBox box = NTextBox(NCursor(), LCD::LINE_WIDTH, LCD::ROWS));

	virtual ~NConsole();

	/** \brief Copy data of buffer into lcd.
	 *
	 * Its recommended to use this not to often because it takes process time.
	 * A good solution is a display update task which has cycle time of about 100 - 400 ms.
	 * Remember the human eye is slow ;)
	 *
	 * @param update If true update the display (hardware).
	 */
	void flush(bool update = false) const {
		this->show(update);
	}

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
	friend NConsole& hex(NConsole& stream);

	/** \brief Ostream manipulator: end actual line and flush console.
	 *
	 * See NOstream::flush() on performance issue.
	 * Use the endl-ascii-char to make "a not flushing endl"
	 *
	 * @param stream
	 * @return
	 */
	friend NConsole& endl(NConsole& stream);

	NConsole& operator<<(const char* str); 		/**<Put C-String into stream.*/
	NConsole& operator<<(char ch);			    /**<Put char into stream.*/
	NConsole& operator<<(S32 num);              /**<Put signed 32bit number into stream.*/
	NConsole& operator<<(U32 num);              /**<Put unsigned 32bit number into stream.*/

	NConsole& operator<<(int num) {return operator<<(static_cast<S32>(num));} /**<Put integer number into stream.*/
	NConsole& operator<<(S16 num) {return operator<<(static_cast<S32>(num));} /**<Put signed 16bit number into stream.*/
	NConsole& operator<<(U16 num) {return operator<<(static_cast<U32>(num));} /**<Put unsigned 32bit number into stream.*/
	NConsole& operator<<(S8 num)  {return operator<<(static_cast<S32>(num));} /**<Put signed 8bit number into stream.*/
	NConsole& operator<<(U8 num)  {return operator<<(static_cast<U32>(num));} /**<Put unsigned 16bit number into stream.*/

	NConsole& operator<<(float num); /**<Put floating-point-number into stream.*/

	NConsole& operator<<(bool b) { /**<Put boolean into stream. Converts bool into text form true/false*/
		if(b) return *this << "true";
		else  return *this << "false";
	}

	NConsole& operator<<(const NString& str) {
		return *this << str.data();
	}

	NConsole& operator<<(NConsole& stream) {return *this;}
	NConsole& operator<<(NOstreamManipulator manip); /**<Put manipulator into stream.*/
};

// manipulator
NConsole& endl(NConsole& stream) {
	stream.mutex.acquire();
	stream.newline();
	stream.mutex.release();
	stream.flush();
	return stream;
}

NConsole& hex(NConsole& stream) {
	stream.nextHex = true;
	return stream;
}

#endif /* __NOSTREAM_HPP_ */
