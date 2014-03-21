/*
 * NOstream_new.cpp
 *
 *  Created on: 21.03.2014
 *      Author: Martin
 */


#include "NOstream_new.hpp"

#include "../../lejos_nxj/src/nxtvm/platform/nxt/display.h"
#include "../../../GNUARM/arm-elf/include/string.h" // strlen
#include "StringConvert.hpp"

namespace nxpl {

// user errors are no problem, we check this when setting cursor position
NOstream::NOstream(mutex_t res, NTextBox box)
: NWidget(box),
  cursorLine(textBox().base().line()),
  somenew(false),
  nextHex(false),
  floatplaces(2),
  mutex(res),
  streamBuffer()
 {
	textBoxValid();
	streamBuffer = new StreamBuffer(textBox().charsInLine(), textBox().lines());
}

NOstream::~NOstream() {
	hide(false);
	delete streamBuffer;
}

void NOstream::textBoxValid() {
	// block user errors, otherwise we may get big arrays and program crashes with new
	if(textBox().lines() > LCD::ROWS || textBox().lines() <= 0) {
		textBox().setLines(LCD::ROWS);
	}
	S16 remaining = LCD::LINE_WIDTH - textBox().base().indent();
	if(remaining < textBox().charsInLine()) {
		textBox().setCharsInLine(remaining);
	}
	if(textBox().charsInLine() > LCD::LINE_WIDTH || textBox().charsInLine() <= 0) {
		textBox().setCharsInLine(LCD::LINE_WIDTH);
	}
}

// make sure that there is space for new line
void NOstream::newLineSpace() {
	if (cursorLine >= textBox().lines()) {
		for (U8 i = 0; i < (textBox().lines()-1); ++i) {
			strcpy(streamBuffer->buffer[i], streamBuffer->buffer[i + 1]);
		}
		strcpy(streamBuffer->buffer[textBox().lines()-1], "");
		cursorLine--;
	}
}

void NOstream::flush(bool update) const { // TODO Continue here
	if(!somenew) return;

	LockGuard lock(mutex);

	if(hasLinesChanged() || hasCharsInLineChanged()) {
		delete streamBuffer;
		streamBuffer = new StreamBuffer(textBox().charsInLine(), textBox().lines());
	}

	// since flush can happen in any code segment with debugging
	// we are not allowed to change cursor position!!
	int save_x = display_get_x();
	int save_y = display_get_y();

	// for every line
	for (U8 i = 0; i < textBox().lines(); ++i) {
		bool lineEnd = false;
		// for every char in line
		for(U8 j = 0; j < textBox().charsInLine(); ++j) {
			if(streamBuffer->buffer[i][j] == '\0') {
				lineEnd = true;
				// jump over termination and continue with whitespace
				continue;
			}
			display_goto_xy(j+textBox().base().indent(), i);
			if(lineEnd) {
				display_char(' ');
			} else {
				display_char(streamBuffer->buffer[i][j]);
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
		// if input is to long we write to new line
		if ((i + j) >= fieldWidth()) {
			textBuffer[cursorLine][fieldWidth()] = '\0';
			newline();
			newLineSpace();
			j = 0;
			i = autoLineFeedIndent;
			continue;
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



