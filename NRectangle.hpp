/*
 * NReactangle.hpp
 *
 *  Created on: 01.12.2013
 *      Author: Martin
 */

#ifndef __RECTANGLE_HPP_
#define __RECTANGLE_HPP_

#include "NShape.hpp"
#include "LcdDrawer.hpp"

namespace nxpl {

class NRectangle : public NShape {
private:

public:

	NRectangle(NLcd *nlcd, const S8 x0, const S8 y0, const S8 w, const S8 h);
	~NRectangle() {}

	void setPosition(const S8 x0 = keep, const S8 y0 = keep, const S8 w = keep, const S8 h = keep);
	void fill() 	 const;
	void fillErase() const;
	void fillInvert() const;

private:
	void showImpl(bool update) const;
	void eraseImpl(bool update) const;
	void invertImpl(bool update) const;
};

NRectangle::NRectangle(NLcd *nlcd, const S8 x0, const S8 y0, const S8 w, const S8 h)
	: NShape(nlcd) {
	setPixelField(x0, y0, w, h);
}


void NRectangle::setPosition(const S8 x0, const S8 y0, const S8 w, const S8 h) {
	if (isVisible()) {
		this->erase();
	}
	if (x0 != keep)
		x(x0);
	if (y0 != keep)
		y(y0);
	if (width() != keep)
		width(w);
	if (height() != keep)
		height(h);
}




void NRectangle::showImpl(bool update) const {
	nxpl::drawRectangle(*lcd, x(), y(), width(), height(), DrawOpt::draw());
}

void NRectangle::eraseImpl(bool update) const {
	nxpl::drawRectangle(*lcd, x(), y(), width(), height(), DrawOpt::clear());
}


void NRectangle::invertImpl(bool update) const {
	nxpl::drawRectangle(*lcd, x(), y(), width(), height(), DrawOpt::invert());
}

void NRectangle::fill() const {
	if(lcd == 0) {
		return;
	}
	// TODO Try if -height/width works!
	nxpl::drawRectangleFilled(*lcd, x(), y(), width(), height(), DrawOpt::draw());
}

void NRectangle::fillErase() const {
	if(lcd == 0) {
		return;
	}
	nxpl::drawRectangleFilled(*lcd, x(), y(), width(), height(), DrawOpt::clear());
}


void NRectangle::fillInvert() const {
	if(lcd == 0) {
		return;
	}
	nxpl::drawRectangleFilled(*lcd, x(), y(), width(), height(), DrawOpt::invert());
}

}

#endif /* NREACTANGLE_HPP_ */
