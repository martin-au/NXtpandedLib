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

class NLine : public NShape {
private:

public:
	explicit NLine(NLcd *nlcd, const S8 x0, const S8 y0, const S8 x1, const S8 y1);
	NLine();
	~NLine() {}
	void setPosition(const S8 x0 = keep, const S8 y0 = keep, const S8 x1 = keep, const S8 y1 = keep);
	S8 endX() const {
		return x + width;
	}
	S8 endY() const {
		return y + height;
	}

private:
	void showImpl(bool update) const;
	void eraseImpl(bool update) const;
	void invertImpl(bool update) const;
};


NLine::NLine(NLcd *nlcd, const S8 x0, const S8 y0, const S8 x1, const S8 y1)
 : NShape(nlcd)
{
	setPixelField(x0, y0, (x1-x0), (y1-y0));
	visible = false;
}


/*
NLine::NLine()
	: endX(0), endY(0)
{
	visible = false;
}
*/


void NLine::setPosition(const S8 x0, const S8 y0, const S8 x1, const S8 y1) {
	if(isVisible()) {
		this->erase();
	}
	if (x0 != keep)
		this->x = x0;
	if (y0 != keep)
		this->y = y0;
	if (x1 != keep)
		width = x1 - x;
	if (y1 != keep)
		height = y1 - y;
}


void NLine::showImpl(bool update) const {
	Drawer::drawLine(*lcd, x, y, endX(), endY(), DrawOpt::draw());
}


void NLine::eraseImpl(bool update) const {
	Drawer::drawLine(*lcd, x, y, endX(), endY(), DrawOpt::clear());
}


void NLine::invertImpl(bool update) const {
	Drawer::drawLine(*lcd, x, y, endX(), endY(), DrawOpt::invert());
}


#endif /* NLINE_HPP_ */
