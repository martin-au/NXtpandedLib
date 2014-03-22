/*
 * NCursor.hpp
 *
 *  Created on: 19.03.2014
 *      Author: Martin
 */

#ifndef __NCURSOR_HPP_
#define __NCURSOR_HPP_

#include "NString.hpp"

namespace nxpl {

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

	void setIndent(S8 indent) {
		indentVal = indent;
	}

	void setLine(S8 line) {
		lineVal = line;
	}

	void moveForward() {
		indentVal++;
	}

	void moveBackward() {
		indentVal--;
	}

	void moveNextLine() {
		lineVal++;
	}

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

	NString asString() const {
		return NString("HEY");
	}
};


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
