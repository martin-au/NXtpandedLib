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

	NRectangle(NLcd *nlcd, S8 x0, S8 y0, S8 w, S8 h) :
			NShape(nlcd) {
		setPixelField(x0, y0, w, h);
	}
	~NRectangle() {}

	void setPosition(S8 x0 = keep, S8 y0 = keep, S8 w = keep, S8 h = keep);
	void fill() const {
		if (lcd == 0) {
			return;
		}
		// TODO Try if -height/width works!
		nxpl::drawRectangleFilled(*lcd, x(), y(), width(), height(),
				DrawOpt::draw());
	}

	void fillErase() const {
		if (lcd == 0) {
			return;
		}
		nxpl::drawRectangleFilled(*lcd, x(), y(), width(), height(),
				DrawOpt::clear());
	}

	void fillInvert() const {
		if (lcd == 0) {
			return;
		}
		nxpl::drawRectangleFilled(*lcd, x(), y(), width(), height(),
				DrawOpt::invert());
	}

private:
	void showImpl(bool update) const {
		nxpl::drawRectangle(*lcd, x(), y(), width(), height(), DrawOpt::draw());
	}

	void eraseImpl(bool update) const {
		nxpl::drawRectangle(*lcd, x(), y(), width(), height(),
				DrawOpt::clear());
	}

	void invertImpl(bool update) const {
		nxpl::drawRectangle(*lcd, x(), y(), width(), height(),
				DrawOpt::invert());
	}
};


void NRectangle::setPosition(S8 x0, S8 y0, S8 w, S8 h) {
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

}

#endif /* NREACTANGLE_HPP_ */
