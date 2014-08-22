/*
 * NPixelBox.hpp
 *
 *  Created on: 19.03.2014
 *      Author: Martin
 */

#ifndef __NPIXELBOX_HPP_
#define __NPIXELBOX_HPP_

#include "NPoint.hpp"
#include "../../../GNUARM/arm-elf/include/stdlib.h"

namespace nxpl {

class NPixelBox {
public:
	NPoint baseLeftTop;
	U16 widthVal, heightVal;

	NPixelBox(NPoint base = NPoint(0, 0), U16 width = 1, U16 height = 1)
	: baseLeftTop(base), widthVal(width), heightVal(height) {
	}

	/** diagonal to base means the point is right, bottom to base
	 *
	 * @param base
	 * @param diagonalToBase
	 */
	NPixelBox(NPoint base, NPoint diagonalToBase)
	: baseLeftTop(base),
	  widthVal(abs(diagonalToBase.x()-base.x())),
	  heightVal(abs(diagonalToBase.y()-base.y())) {

	}

	NPoint base() const {
		return baseLeftTop;
	}

	void setBase(NPoint base) {
		baseLeftTop = base;
	}

	void setBaseX(S16 x) {
		baseLeftTop.setX(x);
	}

	void setBaseY(S16 y) {
		baseLeftTop.setY(y);
	}

	NPoint diagonalToBasePoint() const {
		return baseLeftTop + NPoint(widthVal, heightVal);
	}

	void setDiagonalToBasePoint(NPoint p) {
		  widthVal = abs(p.x()- baseLeftTop.x());
		  heightVal = abs(p.y()- baseLeftTop.y());
	}

	U16 width() const {
		return widthVal;
	}

	void setWidth(U16 width) {
		widthVal = width;
	}

	U16 height() const {
		return heightVal;
	}

	void setHeight(U16 height) {
		heightVal = height;
	}
};

bool pixelBoxInLcd(const NPixelBox &box) {
	return LCD::objectInLcd(box.base().x(), box.base().y(), box.height(), box.width());
}

}


#endif /* NPIXELBOX_HPP_ */
