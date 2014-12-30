/*
 * NCursor.hpp
 *
 *  Created on: 19.03.2014
 *      Author: Martin
 */

#ifndef __NCURSOR_HPP_
#define __NCURSOR_HPP_

#include "NString.hpp"
#include "LcdConstants.hpp"

namespace nxpl {

/**
 * \brief Class for positioning text.
 *
 * The position of a text (on the lcd screen) is described by indent (x) and line (y).
 * One indent and one line is the space occupied by one character.
 * See LcdConstants.hpp for limits on nxt lcd.
 */
class NCursor {
private:
	S8 indentVal, lineVal;
public:
	NCursor(S8 indent = 0, S8 line = 0) : indentVal(indent), lineVal(line) {}
	~NCursor() {}

	S16 indent() const {
		return indentVal;
	}

	S16 line() const {
		return lineVal;
	}

	NCursor setIndent(S8 indent) {
		indentVal = indent;
		return *this;
	}

	NCursor setLine(S8 line) {
		lineVal = line;
		return *this;
	}

	/** \brief Move cursor one character in same line forward */
	void moveForward() {
		indentVal++;
	}

	/** \brief Move cursor one character in same line backward */
	void moveBackward() {
		indentVal--;
	}

	/** \brief Move cursor two next line (down) */
	void moveNextLine() {
		lineVal++;
	}

	/** \brief Move cursor two previous line (up) */
	void movePerviousLine() {
		lineVal--;
	}

	NCursor operator+=(NCursor cursor) {
		indentVal += cursor.indent();
		lineVal += cursor.line();
		return *this;
	}
	NCursor operator-=(NCursor cursor) {
		indentVal -= cursor.indent();
		lineVal -= cursor.line();
		return *this;
	}

	/** \brief Practical function to write cursor as string
	 *
	 * Format: "(indent,line)"
	 * Example: (2,1)
	 */
	NString asString() const {
		const U8 countDigits = 4;
		const U8 countChars = 3;
		NString ret(countChars+countDigits);
		ret.append('(');
		ret.append(indentVal);
		ret.append(',');
		ret.append(lineVal);
		ret.append(')');
		return ret;
	}
};

/** \brief Check if cursor is in nxt lcd */
bool cursorInLcd(NCursor c) {
	return LCD::cursorInLcd(c.indent(), c.line());
}

bool operator!=(NCursor p1, NCursor p2) {
	return p1.indent() != p2.indent() && p1.line() != p2.line();
}

bool operator==(NCursor p1, NCursor p2) {
	return p1.indent() == p2.indent() && p1.line() == p2.line();
}

const NCursor operator+(NCursor p1, NCursor p2) {
	return NCursor(p1.indent() + p2.indent(), p2.line() + p2.line());
}

const NCursor operator-(NCursor p1, NCursor p2) {
	return NCursor(p1.indent() - p2.indent(), p2.line() - p2.line());
}

/*
NOstream& operator<<(NOstream &stream, NCursor &point) {
	return stream << '(' << point.indent() << "," << point.line() << ')';
}
*/

}

#endif /* NCURSOR_HPP_ */
