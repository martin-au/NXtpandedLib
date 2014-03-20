/*
 * NCursor.hpp
 *
 *  Created on: 19.03.2014
 *      Author: Martin
 */

#ifndef __NCURSOR_HPP_
#define __NCURSOR_HPP_

#include "NOstream.hpp"

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

	NCursor operator+=(NCursor point) {
		indentVal += point.indent();
		lineVal += point.line();
		return *this;
	}
	NCursor operator-=(NCursor point) {
		indentVal -= point.indent();
		lineVal -= point.line();
		return *this;
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

NOstream& operator<<(NOstream &stream, NCursor &point) {
	return stream << '(' << point.indent() << "," << point.line() << ')';
}

}

#endif /* NCURSOR_HPP_ */
