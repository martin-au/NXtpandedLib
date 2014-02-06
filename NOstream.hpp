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

//for codecorrection

#include "C:/cygwin/nxtOSEK/lejos_nxj/src/nxtvm/platform/nxt/display.h"

#include "C:/cygwin/GNUARM/arm-elf/include/string.h" // strlen

#include "convert.hpp"

#include "Uncopyable.hpp"
#include "Mutex.hpp"

// TODO Make a line type class?

class NOstream : private Uncopyable
{
private:
	// Actual cursor Line
	unsigned int cursorLine;
	// boarders
	unsigned int startLine;
	unsigned int lastLine;
	unsigned int x;
	unsigned int width;
	bool somenew;
	bool nextHex;
	U16 floatplaces;

	mutex_t mutex;

	// Console buffer array
	char **textBuffer;

	// allowing manipulators (endl, hex ...)
	typedef NOstream& (*NOstreamManipulator)(NOstream&);

	// cursor in console area
	inline bool inArea(const U16 x, const U16 y) const;

	// set cursor position
	void cursor(U16 x, U16 y) const;

	void newline();

	// the only way to put data into the textBuffer
	// responsible for text formatting...
	void streamhandler(const char *str);
public:

	// Max cursor position in X(horizontal) axis.
	static const unsigned int MAX_CURSOR_X = 15;
    // Max cursor position in Y(vertical) axis.
	static const unsigned int MAX_CURSOR_Y = 7;

	explicit NOstream(mutex_t res,
			 const U16 startLine = 0,
			 const U16 lastLine = MAX_CURSOR_Y,
			 const U16 x = 0,
			 const U16 width = 16);
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
	NOstream& operator<<(float num);


	/*
	template<typename T>
	NOstream& operator<<(T num) {
		if (num < 0)
			return operator<<(static_cast<S32>(num));
		else
			return operator<<(static_cast<U32>(num));
	}
	*/


	NOstream& operator<<(NOstream& stream) {return *this;}
	NOstream& operator<<(NOstreamManipulator manip);
};


bool NOstream::inArea(const U16 x, const U16 y) const {
	return (y >= startLine) && (y <= lastLine) && (x >= (this->x))
			&& (x <= (width + (this->x) - 1));
}



// easier
#include "NOstream.cpp"



#endif /* __OSTREAM_H_ */
