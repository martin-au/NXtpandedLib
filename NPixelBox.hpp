/*
 * NPixelBox.hpp
 *
 *  Created on: 19.03.2014
 *      Author: Martin
 */

#ifndef __NPIXELBOX_HPP_
#define __NPIXELBOX_HPP_

#include "NPoint.hpp"

#include "..\..\..\GNUARM\include\c++\4.0.2\cstdlib" //abs

namespace nxpl {

class NPixelBox {
public:
	NPoint baseLeftTop;
	U16 widthVal, heightVal;

	NPixelBox(NPoint base, U16 width, U16 height)
	: baseLeftTop(base), widthVal(width), heightVal(height) {
	}

	/** diagonal to base means the point is right, bottom to base
	 *
	 * @param base
	 * @param diagonalToBase
	 */
	NPixelBox(NPoint base, NPoint diagonalToBase)
	: baseLeftTop(base),
	  widthVal(std::abs(diagonalToBase.x()-base.x())),
	  heightVal(std::abs(diagonalToBase.y()-base.y())) {

	}

	NPoint base() const {
		return baseLeftTop;
	}

//	NPoint & base()  {
//		return &baseLeftTop;
//	}

	void setBase(NPoint base) {
		baseLeftTop = base;
	}

	NPoint diagonalToBasePoint() const {
		return baseLeftTop + NPoint(widthVal, heightVal);
	}

	void setDiagonalToBasePoint(NPoint p) {
		  widthVal = std::abs(p.x()- baseLeftTop.x());
		  heightVal = std::abs(p.y()- baseLeftTop.y());
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



}


#endif /* NPIXELBOX_HPP_ */
