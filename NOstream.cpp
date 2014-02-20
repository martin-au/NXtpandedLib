
#include "NOstream.hpp"
#include "C:/cygwin/nxtOSEK/lejos_nxj/src/nxtvm/platform/nxt/display.h"
#include "C:/cygwin/GNUARM/arm-elf/include/string.h" // strlen
#include "StringConvert.hpp"

namespace nxpl {

// user errors are no problem, we check this when setting cursor position
NOstream::NOstream(mutex_t res,
				   U8 startLine,
				   U8 rows,
				   U8 indent,
				   U8 lineWidth)
: cursorLine(startLine),
  somenew(false),
  nextHex(false),
  floatplaces(2),
  mutex(res)
 {
	// block user errors, otherwise we may get big arrays and program crashes with new
	rows = (rows > LCD::ROWS) ? LCD::ROWS : rows;
	S16 remaining = LCD::LINE_WIDTH - indent;
	lineWidth = (remaining < lineWidth) ? remaining : lineWidth;
	if(lineWidth > LCD::LINE_WIDTH) lineWidth = LCD::LINE_WIDTH;

	setField(indent, startLine, rows, lineWidth);

	textBuffer = new char *[lines()];
	for (int i = 0; i < rows + 1; i++) {
		textBuffer[i] = new char[fieldWidth()+1]; // +1 for '\0'
		strcpy(textBuffer[i], "");
	}
}

NOstream::~NOstream() {
	hide();
	for (int i = 0; i < lines() + 1; i++) {
		delete[] textBuffer[i];
	}
	delete[] textBuffer;
}


// make sure that there is space for new line
void NOstream::newLineSpace() {
	if (cursorLine >= lines()) {
		for (U8 i = 0; i < (lines()-1); ++i) {
			strcpy(textBuffer[i], textBuffer[i + 1]);
		}
		strcpy(textBuffer[lines()-1], "");
		cursorLine--;
	}
}

void NOstream::flush(bool update) const {
	if(!inLcd() || !somenew) return;

	LockGuard lock(mutex);

	// since flush can happen in any code segment with debugging
	// we are not allowed to change cursor position!!
	int save_x = display_get_x();
	int save_y = display_get_y();
	// and yes, its still not task save: there should be no cursor change until goto.

	// for every line
	for (U8 i = 0; i < lines(); ++i) {
		bool lineEnd = false;
		// for every char in line
		for(U8 j = 0; j < fieldWidth(); ++j) {
			if(textBuffer[i][j] == '\0') {
				lineEnd = true;
				// jump over termination and continue with whitespace
				continue;
			}
			display_goto_xy(j+indent(), i);
			if(lineEnd) {
				display_char(' ');
			} else {
				display_char(textBuffer[i][j]);
			}
		}
	}
	somenew = false;
	display_goto_xy(save_x, save_y);
	if(update) {
		display_update();
	}
	setVisibility(true);
}

void NOstream::hide(bool update) const {
	if(!inLcd()) return;

	int save_x = display_get_x();
	int save_y = display_get_y();

	U8 last = indent() + fieldWidth();
	for (U8 i = 0; i < lines(); ++i) {
		for(U8 j = indent(); j < last; ++j) {
			display_goto_xy(j, i);
			display_char(' ');
		}
	}
	display_goto_xy(save_x, save_y);
	if(update) {
		display_update();
	}
	setVisibility(false);
}

U16 NOstream::precision() const {
	return floatplaces;
}

U16 NOstream::precision(U16 prec) {
	floatplaces = prec;
	return floatplaces;
}

void NOstream::streamhandler(const char *str) {
	newLineSpace();
	// i..idx of first free char in textBuffer
	// j..idx iterator of destination string
	// k..idx iterator of source string
	LockGuard lock(mutex);
	U16 j = 0;
	U16 k = 0;
	U16 i = strlen(textBuffer[cursorLine]);
	for (; str[k] != '\0'; j++) {
		if ((i + j) >= fieldWidth()) {
			// cutoff or line feed?
			// textBuffer[cursorLine][fieldWidth()] = '\0';
			break;
		}
		if (str[k] == '\n') {
			textBuffer[cursorLine][i + j] = '\0';
			newline();
			newLineSpace();
			j = 0;
			i = 0;
		} else {
			textBuffer[cursorLine][i + j] = str[k];
		}
		k++;
	}
	textBuffer[cursorLine][i + j] = '\0';

	somenew = true;
}


// manipulator

NOstream& endl(NOstream& stream) {
	stream.mutex.acquire();
	stream.newline();
	stream.mutex.release();
	stream.flush();
	return stream;
}

NOstream& hex(NOstream& stream) {
	stream.nextHex = true;
	return stream;
}

NOstream& NOstream::operator<<(const char* str) {
	streamhandler(str);
	return *this;
}

NOstream& NOstream::operator<<(char str) {
	char tmp[2];
	tmp[0] = str;
	tmp[1] = '\0';
	streamhandler(tmp);
	return *this;
}

NOstream& NOstream::operator<<(S32 num) {
	if (!nextHex) {
		char str[12];
		numToStr(num, str);
		streamhandler(str);
	} else {
		char str[8];
		numToHex(num, str);
		streamhandler(str);
		nextHex = false;
	}
	return *this;
}

NOstream& NOstream::operator<<(U32 num) {
	if (!nextHex) {
		char str[11];
		numToStr(num, str);
		streamhandler(str);
	} else {
		char str[8];
		numToHex(num, str);
		streamhandler(str);
		nextHex = false;
	}
	return *this;
}

NOstream& NOstream::operator<<(float num) {
	//putf("f", num);
	char str[12];
	numToStr(num, str, floatplaces);
	streamhandler(str);
	return *this;
}

NOstream& NOstream::operator<<(NOstreamManipulator manip) {
	return manip(*this);
}

}
