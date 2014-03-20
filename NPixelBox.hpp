/*
 * NPixelBox.hpp
 *
 *  Created on: 19.03.2014
 *      Author: Martin
 */

#ifndef __NPIXELBOX_HPP_
#define __NPIXELBOX_HPP_

#include "NPoint.hpp"

namespace nxpl {

class NPixelBox {
public:
	NPoint baseLeftTop;
	S16 widthVal, heightVal;

	NPixelBox(NPoint baseLeftTop, S16 width, S16 height)
	: baseLeftTop(baseLeftTop), widthVal(width), heightVal(height) {

	}

	NPoint base() const {
		return baseLeftTop;
	}

	NPoint & base()  {
		return &baseLeftTop;
	}

	void setBase(NPoint base) {
		baseLeftTop = base;
	}

	S16 width() const {
		return width;
	}

	void setWidth(S16 width) const {
		widthVal = width;
	}

	S16 height() const {
		return heightVal;
	}

	void setHeight(S16 height) {
		heightVal = height;
	}
};



}


#endif /* NPIXELBOX_HPP_ */
