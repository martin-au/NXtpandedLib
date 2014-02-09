/*
 * NOstream
 *
 *  Created on: 30.09.2013
 *      Author: Martin
 */

#ifndef __OSTREAM_H_
#define __OSTREAM_H_

extern "C" {
	#include "C:/cygwin/nxtOSEK/toppers_osek/include/kernel.h"
	#include "kernel_id.h"
	#include "C:/cygwin/nxtOSEK/ecrobot/c/ecrobot_interface.h"
	#include "C:/cygwin/nxtOSEK/lejos_nxj/src/nxtvm/platform/nxt/display.h"
}

#include "LcdConstants.hpp"
#include "C:/cygwin/nxtOSEK/lejos_nxj/src/nxtvm/platform/nxt/display.h"

#include "C:/cygwin/GNUARM/arm-elf/include/string.h" // strlen

#include "convert.hpp"

#include "NWidget.hpp"
#include "Uncopyable.hpp"
#include "Mutex.hpp"


class NOstream : public NWidget, private Uncopyable
{
private:
	// Actual cursor Line
	U8 cursorLine;
	// boarders
	//U8 startLine;
	//U8 consoleRows;
	//U8 x;
	//U8 width;
	bool somenew;
	bool nextHex;
	U16 floatplaces;

	mutex_t mutex;

	// Console buffer array
	char **textBuffer;

	// allowing manipulators (endl, hex ...)
	typedef NOstream& (*NOstreamManipulator)(NOstream&);

	// cursor in console area
	inline bool inArea(U16 x, U16 y) const;

	// set cursor position
	//bool cursor(U16 x, U16 y) const;

	void newline() {
		++cursorLine;
	}
	void newLineSpace();

	// the only way to put data into the textBuffer
	// responsible for text formatting...
	void streamhandler(const char *str);
public:

	// Max cursor position in X(horizontal) axis.
	static const U8 MAX_CURSOR_X = LCD::LINE_WIDTH - 1;
    // Max cursor position in Y(vertical) axis.
	static const U8 MAX_CURSOR_Y = LCD::ROWS - 1;

	explicit NOstream(mutex_t res,
			 	      U8 startLine = 0,
			          U8 rows = LCD::ROWS,
			          U8 x = 0,
			          U8 width = LCD::LINE_WIDTH);
	~NOstream();

	// do not allow copy

	void flush();
	void hide(bool update = false) const;

	U16 precision() const;
	U16 precision(U16 prec);

	friend NOstream& hex(NOstream& stream);
	friend NOstream& endl(NOstream& stream);

	NOstream& operator<<(const char* str);
	NOstream& operator<<(char str);
	NOstream& operator<<(S32 num);
	NOstream& operator<<(U32 num);

	NOstream& operator<<(S16 num) {return operator<<(static_cast<S32>(num));}
	NOstream& operator<<(U16 num) {return operator<<(static_cast<U32>(num));}
	NOstream& operator<<(S8 num)  {return operator<<(static_cast<S32>(num));}
	NOstream& operator<<(U8 num)  {return operator<<(static_cast<U32>(num));}

	NOstream& operator<<(float num);

	NOstream& operator<<(bool b) {
		if(b) return *this << "true";
		else  return *this << "false";
	}

	NOstream& operator<<(NOstream& stream) {return *this;}
	NOstream& operator<<(NOstreamManipulator manip);
};

/*
bool NOstream::inArea(U16 x, U16 y) const {
	return (y >= startLine) && (y < consoleRows) && (x >= (this->x))
			&& (x < (width + (this->x)));
}
*/

/*
// Set cursor to (x,y) position. Top left is (0,0).
bool NOstream::cursor(U16 x, U16 y) const {
	x = (x > MAX_CURSOR_X) ? MAX_CURSOR_X : x;
	y = (y > MAX_CURSOR_Y) ? MAX_CURSOR_Y : y;
	if (inArea(x, y)) {
		display_goto_xy(static_cast<int>(x), static_cast<int>(y));
		return true;
	}
	return false;
}
*/


// easier
#include "NOstream.cpp"



#endif /* __OSTREAM_H_ */
