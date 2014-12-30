/*
 * NOstream_new.cpp
 *
 *  Created on: 21.03.2014
 *      Author: Martin
 */


#include "NConsole.hpp"

#include "../../lejos_nxj/src/nxtvm/platform/nxt/display.h"
#include "../../../GNUARM/arm-elf/include/string.h" // strlen
#include "StringConvert.hpp"

// user errors are no problem, we check this when setting cursor position
NConsole::NConsole(NMutex res, NTextBox box)
: NWidget(box),
  somenew(false),
  nextHex(false),
  floatplaces(2),
  mutex(res),
  streamBuffer()
 {
	textBoxValid();
	streamBuffer = new StreamBuffer(textBox().charsInLine(), textBox().lines());
}

NConsole::~NConsole() {
	hide(false);
	delete streamBuffer;
}

void NConsole::textBoxValid() {
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
void NConsole::newLineSpace() {
	if (streamBuffer->cursorLine >= textBox().lines()) {
		for (U8 i = 0; i < (textBox().lines()-1); ++i) {
			strcpy(streamBuffer->buffer[i], streamBuffer->buffer[i + 1]);
		}
		strcpy(streamBuffer->buffer[textBox().lines()-1], "");
		(streamBuffer->cursorLine)--;
	}
}

void NConsole::textBoxChangedHandler() {
	textBoxValid();
	if(hasLinesChanged() || hasCharsInLineChanged()) {
		delete streamBuffer;
		streamBuffer = new StreamBuffer(textBox().charsInLine(), textBox().lines());
	}
}

void NConsole::showWidgetImpl() const {
	if(!somenew) return;

	LockGuard lock(mutex);
	// since flush can happen in any code segment with debugging
	// we are not allowed to change cursor position!!
	int save_x = display_get_x();
	int save_y = display_get_y();

	// for every line
	bool lineEnd = false;
	for (U8 i = 0; i < textBox().lines(); ++i) {
		lineEnd = false;
		// for every char in line
		for(U8 j = 0; j < textBox().charsInLine(); ++j) {
			if(streamBuffer->buffer[i][j] == '\0') {
				lineEnd = true;
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
}

void NConsole::hideWidgetImpl() const {
	int save_x = display_get_x();
	int save_y = display_get_y();

	U8 last = textBox().base().indent() + textBox().charsInLine();
	for (U8 i = 0; i < textBox().lines(); ++i) {
		for(U8 j = textBox().base().indent(); j < last; ++j) {
			display_goto_xy(j, i);
			display_char(' ');
		}
	}
	display_goto_xy(save_x, save_y);
}

U16 NConsole::precision() const {
	return floatplaces;
}

U16 NConsole::precision(U16 prec) {
	floatplaces = prec;
	return floatplaces;
}

void NConsole::streamhandler(const char *str) {
	newLineSpace();
	// i..idx of first free char in textBuffer
	// j..idx iterator of destination string
	// k..idx iterator of source string
	LockGuard lock(mutex);
	S16 j = 0;
	S16 k = 0;
	S16 i = strlen(streamBuffer->buffer[streamBuffer->cursorLine]);
	for (; str[k] != '\0'; ++j) {
		// if input is to long we write to new line
		if ((i + j) >= textBox().charsInLine()) {
			streamBuffer->buffer[streamBuffer->cursorLine][i + j] = '\0';
			newline();
			newLineSpace();
			j = 0;
			i = autoLineFeedIndent; // make auto line feed visible with indent
			// clear first to chars in line!
			streamBuffer->buffer[streamBuffer->cursorLine][0] = ' ';
			streamBuffer->buffer[streamBuffer->cursorLine][1] = ' ';
		}
		if (str[k] == '\n') {
			streamBuffer->buffer[streamBuffer->cursorLine][i + j] = '\0';
			newline();
			newLineSpace();
			j = -1; // so we start at begin with next iteration
			i = 0;
		} else {
			streamBuffer->buffer[streamBuffer->cursorLine][i + j] = str[k];
			/*
			display_goto_xy(1, 7);
			display_char(streamBuffer->buffer[streamBuffer->cursorLine][i + j]);
			display_goto_xy(5, 7);
			display_int(i+j, 2);
			display_update();
			systick_wait_ms(1000);
			*/
		}
		k++;
	}
	streamBuffer->buffer[streamBuffer->cursorLine][i + j] = '\0';

	somenew = true;
}

NConsole& NConsole::operator<<(const char* str) {
	streamhandler(str);
	return *this;
}

NConsole& NConsole::operator<<(char ch) {
	char tmp[2];
	tmp[0] = ch;
	tmp[1] = '\0';
	streamhandler(tmp);
	return *this;
}

NConsole& NConsole::operator<<(S32 num) {
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

NConsole& NConsole::operator<<(U32 num) {
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

NConsole& NConsole::operator<<(float num) {
	//putf("f", num);
	char str[12];
	numToStr(num, str, floatplaces);
	streamhandler(str);
	return *this;
}

NConsole& NConsole::operator<<(NOstreamManipulator manip) {
	return manip(*this);
}



