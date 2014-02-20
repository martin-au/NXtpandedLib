/*
 * NCircle.hpp
 *
 *  Created on: 01.12.2013
 *      Author: Martin
 */

#ifndef __NCIRCLE_HPP_
#define __NCIRCLE_HPP_


#include "NShape.hpp"
#include "LcdDrawer.hpp"

namespace nxpl {

class NCircle : public NShape {
private:
	S8 r;

	void rasterCircleFill(void (NLcd::*fpPixelState)(const U8, const U8)) const;
public:

	NCircle(NLcd *nlcd, const S8 centerX, const S8 centerY, const S8 radius);
	~NCircle() {}

	S8 centerX() const {
		return x() + r;
	}
	S8 centerY() const {
		return y() + r;
	}

	void setRadius(S8 radius) {
		// clear?
		if(radius <= LCD::WIDTH/2 && radius <= LCD::HEIGHT/2 && r > 0)
			r = radius;
	}

	void setPosition(const S8 centerX = keep, const S8 centerY = keep, const S8 radius = keep);

	void fill() 	 const;
	void fillErase() const;
	void fillInvert() const;

private:
	void showImpl(bool update) const;
	void eraseImpl(bool update) const;
	void invertImpl(bool update) const;
};


NCircle::NCircle(NLcd *nlcd, const S8 centerX, const S8 centerY, const S8 radius)
	: NShape(nlcd), r(radius) {

	setPixelField(centerX - r, centerY - r, 2*r, 2*r);
}


void NCircle::setPosition(const S8 centerX, const S8 centerY, const S8 radius) {
	if(isVisible()) {
		this->erase();
	}
	if (centerX != keep)
		x(centerX - radius);
	if (centerY != keep)
		y(centerY - radius);
	if (width() != keep)
		width(2*radius);
	if (height() != keep)
		height(width());
}



void NCircle::showImpl(bool update) const {
	nxpl::drawCircle(*lcd, centerX(), centerY(), r, DrawOpt::draw());
}

void NCircle::eraseImpl(bool update) const {
	nxpl::drawCircle(*lcd, centerX(), centerY(), r, DrawOpt::clear());
}


void NCircle::invertImpl(bool update) const {
	nxpl::drawCircle(*lcd, centerX(), centerY(), r, DrawOpt::invert());
}


void NCircle::rasterCircleFill(void (NLcd::*fpPixelState)(const U8, const U8)) const {
	S8 cX = centerX();
	S8 cY = centerY();

	for(int y=-r; y<=r; y++) {
	    for(int x=-r; x<=r; x++) {
	        if(x*x+y*y <= r*r)
	            (lcd->*fpPixelState)(cX+x, cY+y);
	    }
	}
	// brute force algo may overwrite outer circle
	if(isVisible()) {
		this->show();
	}
}


void NCircle::fill() const {
	if(lcd == 0) {
		return;
	}
	rasterCircleFill(&NLcd::pixelOn);
}


void NCircle::fillErase() const {
	if(lcd == 0) {
		return;
	}
	rasterCircleFill(&NLcd::pixelOff);
}


void NCircle::fillInvert() const {
	if(lcd == 0) {
		return;
	}
	rasterCircleFill(&NLcd::invertPixel);
}

}

#endif /* NCIRCLE_HPP_ */
