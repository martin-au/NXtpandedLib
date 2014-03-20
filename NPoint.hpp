/*
 * Point.hpp
 *
 *  Created on: 19.03.2014
 *      Author: Martin
 */

#ifndef __NPOINT_HPP_
#define __NPOINT_HPP_

#include "NOstream.hpp"

namespace nxpl {

class NPoint {
private:
	S16 xVal, yVal;
public:
	NPoint(S16 x = 0, S16 y = 0) : xVal(x), yVal(y) {}
	~NPoint() {}

	bool isNull() const {
		return NPoint(0, 0) == *this;
	}

	S16 x() const {
		return this->xVal;
	}

	S16 y() const {
		return this->yVal;
	}

	void setX(S16 x) {
		xVal = x;
	}

	void setY(S16 y) {
		yVal = y;
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
};


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

NOstream& operator<<(NOstream &stream, NPoint &point) {
	return stream << '(' << point.x() << "," << point.y() << ')';
}

}

#endif /* POINT_HPP_ */
