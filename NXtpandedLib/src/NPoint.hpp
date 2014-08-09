/*
 * Point.hpp
 *
 *  Created on: 19.03.2014
 *      Author: Martin
 */

#ifndef __NPOINT_HPP_
#define __NPOINT_HPP_

#include "NString.hpp"
#include "LcdConstants.hpp"
#include "NCursor.hpp"

namespace nxpl {

class NPoint {
private:
	S16 xVal, yVal;
public:
	NPoint(S16 x = 0, S16 y = 0) : xVal(x), yVal(y) {}

	NPoint(NCursor cursor) :
	  xVal(LCD::indentToPixelX(cursor.indent())),
	  yVal(LCD::lineToPixelY(cursor.line())) {
	}

	~NPoint() {}

	bool isNull() const {
		return xVal == 0 && yVal == 0;
	}

	S16 x() const {
		return this->xVal;
	}

	S16 y() const {
		return this->yVal;
	}

	NPoint setX(S16 x) {
		xVal = x;
		return *this;
	}

	NPoint setY(S16 y) {
		yVal = y;
		return *this;
	}

	NPoint operator+=(NPoint point) {
		xVal += point.x();
		yVal += point.y();
		return *this;
	}
	NPoint operator-=(NPoint point) {
		xVal -= point.x();
		yVal -= point.y();
		return *this;
	}

	NString asString() const {
		const U8 countDigits
			= numDigits(static_cast<S32>(xVal))+ numDigits(static_cast<S32>(yVal));
		const U8 numChars = 3;
		NString ret(numChars+countDigits);
		ret.append('(');
		ret.append(xVal);
		ret.append(',');
		ret.append(yVal);
		ret.append(')');
		return ret;
	}
};

bool pointInLcd(NPoint p) {
	return LCD::pixelInLcd(p.x(), p.y());
}

bool operator!=(NPoint p1, NPoint p2) {
	return p1.x() != p2.x() && p1.y() != p2.y();
}

bool operator==(NPoint p1, NPoint p2) {
	return p1.x() == p2.x() && p1.y() == p2.y();
}

const NPoint operator+(NPoint p1, NPoint p2) {
	return NPoint(p1.x() + p2.x(), p2.y() + p2.y());
}

const NPoint operator-(NPoint p1, NPoint p2) {
	return NPoint(p1.x() - p2.x(), p2.y() - p2.y());
}

/*
NOstream& operator<<(NOstream &stream, NPoint &point) {
	return stream << '(' << point.x() << "," << point.y() << ')';
}
*/

}

#endif /* POINT_HPP_ */
