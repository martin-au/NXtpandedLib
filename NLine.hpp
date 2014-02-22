/*
 * NLine.hpp
 *
 *  Created on: 23.11.2013
 *      Author: Martin
 */

#ifndef __NLINE_HPP_
#define __NLINE_HPP_


#include "NShape.hpp"
#include "LcdDrawer.hpp"

namespace nxpl {

class NLine : public NShape {
private:

public:
	explicit NLine(NLcd *nlcd, S8 x0, S8 y0, S8 x1, S8 y1) :
			NShape(nlcd) {
		setPixelField(x0, y0, (x1 - x0), (y1 - y0));
	}
	NLine();
	~NLine() {}
	void setPosition(S8 x0 = keep, S8 y0 = keep, S8 x1 = keep, S8 y1 = keep);
	S8 endX() const {
		return x() + width();
	}
	S8 endY() const {
		return y() + height();
	}

private:
	void showImpl(bool update) const {
		nxpl::drawLine(*lcd, x(), y(), endX(), endY(), DrawOpt::draw());
	}


	void eraseImpl(bool update) const {
		nxpl::drawLine(*lcd, x(), y(), endX(), endY(), DrawOpt::clear());
	}


	void invertImpl(bool update) const {
		nxpl::drawLine(*lcd, x(), y(), endX(), endY(), DrawOpt::invert());
	}
};


void NLine::setPosition(S8 x0, S8 y0, S8 x1, S8 y1) {
	if(isVisible()) {
		this->erase();
	}
	if (x0 != keep)
		x(x0);
	if (y0 != keep)
		y(y0);
	if (x1 != keep)
		width(x1 - x());
	if (y1 != keep)
		height(y1 - y());
}

}

#endif /* NLINE_HPP_ */
