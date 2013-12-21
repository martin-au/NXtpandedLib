

#include "ostream.hpp"

ostream::ostream(mutex_t res,
				 const U16 startLine,
				 const U16 lastLine,
				 const U16 x,
				 const U16 width)
: cursorLine(startLine),
  startLine(startLine),
  lastLine(lastLine),
  x(x),
  width(width), // absichern!!
  somenew(0),
  nextHex(0),
  floatplaces(2),
  mutex(res)
 {
	// get width to end of display
	S16 remaining = (MAX_CURSOR_X + 1) - x; // +1 because 0 do not count
	if (remaining >= width) {
		// from x to end of line
		this->width = width; //+1 for array
	} else {
		this->width = remaining;
	}

	int rows = lastLine + 1 - startLine;
	textBuffer = new char *[rows];

	for (int i = 0; i < rows + 1; i++) {
		textBuffer[i] = new char[width];
		strcpy(textBuffer[i], "");
	}
}

ostream::~ostream() {
	int rows = lastLine + 1 - startLine;
	for (int i = 0; i < rows + 1; i++) {
		delete[] textBuffer[i];
	}
	delete[] textBuffer;
}

bool ostream::inArea(const U16 x, const U16 y) const {
	return (y >= startLine) && (y <= lastLine) && (x >= (this->x))
			&& (x <= (width + (this->x) - 1));
}



// Set cursor to (x,y) position. Top left is (0,0).
void ostream::cursor(U16 x, U16 y) {
	x = (x > MAX_CURSOR_X) ? MAX_CURSOR_X : x;
	y = (y > MAX_CURSOR_Y) ? MAX_CURSOR_Y : y;
	if (inArea(x, y)) {
		display_goto_xy(static_cast<int>(x), static_cast<int>(y));
	}
}

void ostream::newline() {
	++cursorLine;
	// moving out of lcd
	if (cursorLine > (lastLine + 1)) {
		for (unsigned int i = 0; i <= lastLine; ++i) {
			strcpy(textBuffer[i], textBuffer[i + 1]);
		}
		strcpy(textBuffer[lastLine + 1], ""); // do we need this? textBuffer[lastLine+1][0] = '\0'
		cursorLine--;
	}
}

void ostream::flush() {
	if (!somenew)
		return;   // do not update display if there is nothing new!
	LockGuard lock(mutex);

	// since flush can happen in any code segment with debugging
	// we are not allowed to change cursor position!!
	int save_x = display_get_x();
	int save_y = display_get_y();
	// and yes, its still not task save: there should be no cursor change until goto.

	for (unsigned int i = 0; i <= lastLine; ++i) {
		for(unsigned int j = x; j<(x+width); ++j) {
			cursor(j, i);
			// clean full line!
			display_char( (textBuffer[i][j] != '\0')? (textBuffer[i][j]) : ' ');
		}
	}
	somenew = false;
	display_goto_xy(save_x, save_y);
	display_update();
}

U16 ostream::precision() const {
	return floatplaces;
}

U16 ostream::precision(U16 prec) {
	floatplaces = prec;
	return floatplaces;
}

void ostream::streamhandler(const char *str) {
	// i..end of textBuffer pre
	// j..copy to string dest
	// k..copy from string src
	LockGuard lock(mutex);
	U16 i, j, k;
	k = 0;
	i = strlen(textBuffer[cursorLine]);
	for (j = 0; str[k] != '\0'; j++) {
		if ((i + j) >= width) {
			// cutoff or line feed?
			// textBuffer[cursorLine][width] = '\0';
			break;
		}
		if (str[k] == '\n') {
			textBuffer[cursorLine][i + j] = '\0';
			newline();
			//k++; // jump over
			j = 0;
			i = 0; // we assume that there is nothing in next line
		} else {
			textBuffer[cursorLine][i + j] = str[k];
		}
		k++;
	}
	textBuffer[cursorLine][i + j] = '\0';

	somenew = true;
}


// manipulator

ostream& endl(ostream& stream) {
	stream.mutex.Acquire();
	stream.newline();
	stream.mutex.Release();
	stream.flush();
	return stream;
}

ostream& hex(ostream& stream) {
	stream.nextHex = true;
	return stream;
}

ostream& ostream::operator<<(const char* str) {
	streamhandler(str);
	return *this;
}

ostream& ostream::operator<<(char str) {
	char tmp[2];
	tmp[0] = str;
	tmp[1] = '\0';
	streamhandler(tmp);
	return *this;
}

ostream& ostream::operator<<(S32 num) {
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

ostream& ostream::operator<<(U32 num) {
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

ostream& ostream::operator<<(float num) {
	//putf("f", num);
	char str[12];
	numToStr(num, str, floatplaces);
	streamhandler(str);
	return *this;
}

ostream& ostream::operator<<(ostreamManipulator manip) {
	return manip(*this);
}

