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

#include "NWidget.hpp"
#include "Uncopyable.hpp"
#include "Mutex.hpp"

// TODO new function show because this is also a NWidget?
namespace nxpl {

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
	explicit NOstream(mutex_t res,
			  U8 startLine = 0,
			  U8 rows = LCD::ROWS,
			  U8 x = 0,
			  U8 width = LCD::LINE_WIDTH);
	~NOstream();

	void flush(bool update = false) const;

	virtual void show(bool update = false) const {
		flush(update);
	}
	virtual void hide(bool update = false) const;

	U16 precision() const;
	U16 precision(U16 prec);

	friend NOstream& hex(NOstream& stream);
	friend NOstream& endl(NOstream& stream);

	NOstream& operator<<(const char* str);
	NOstream& operator<<(char str);
	NOstream& operator<<(S32 num);
	NOstream& operator<<(U32 num);

	NOstream& operator<<(int num) {return operator<<(static_cast<S32>(num));}
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

}

#endif /* __OSTREAM_H_ */
