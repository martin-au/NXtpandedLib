/*
 * NCircle.hpp
 *
 *  Created on: 01.12.2013
 *      Author: Martin
 */

#ifndef __NCIRCLE_HPP_
#define __NCIRCLE_HPP_

/** \file
 *	\ingroup NxtLcd
*/

#include "NShape.hpp"
#include "LcdDrawer.hpp"

namespace nxpl {

/** \brief Circle object for lcd.
 */
class NCircle : public NShape {
private:
	S8 r;

	void rasterCircleFill(void (NLcd::*fpPixelState)(U8, U8)) const;
public:

	/** \brief Construct a circle object.
	 *
	 * This constructs a circle on the given lcd with its center at the specified x and y location, using the specified radius.
	 *
	 * @param nlcd      The lcd for drawing the circle.
	 * @param centerX  The x value for the center of the circle.
	 * @param centerY  The y value for the center of the circle.
	 * @param radius   The radius of the circle.
	 */
	NCircle::NCircle(NLcd *nlcd, S8 centerX, S8 centerY, S8 radius) :
			NShape(nlcd), r(radius) {
		setPixelField(centerX - r, centerY - r, 2 * r, 2 * r);
	}

	/** \brief Destructor
	 *  Hides line if visible.
	 */
	virtual ~NCircle() {
		if(isVisible()) {
			hide();
		}
	}

	/** \brief Calculate the x-coordinate of the center point.
	 *
	 * @return Center point x-coordinate
	 */
	S8 centerX() const {
		return x() + r;
	}

	/** \brief Calculate the y-coordinate of the center point.
	 *
	 * @return Center point y-coordinate
	 */
	S8 centerY() const {
		return y() + r;
	}

	/** \brief Set circle radius.
	 *
	 * @param radius New radius.
	 */
	void setRadius(S8 radius) {
		if(radius <= LCD::WIDTH/2 && radius <= LCD::HEIGHT/2 && r > 0)
			r = radius;
	}

	/** \brief Set position of the circle.
	 *
	 * Use NWidget::keep to keep the actual value.
	 *
	 * @param centerX  Center point x-coordinate
	 * @param centerY  Center point y-coordinate
	 * @param radius   New radius.
	 */
	void setPosition(S8 centerX = keep, S8 centerY = keep, S8 radius = keep);

	/** \brief Fill the circle.
	 */
	void fill() const {
		if (lcd == 0) {
			return;
		}
		rasterCircleFill(&NLcd::pixelOn);
	}

	/** \brief Erase the fill of the circle.
	 */
	void fillErase() const {
		if (lcd == 0) {
			return;
		}
		rasterCircleFill(&NLcd::pixelOff);
	}

	/** \brief Invert the fill of the circle.
	 */
	void fillInvert() const {
		if (lcd == 0) {
			return;
		}
		rasterCircleFill(&NLcd::invertPixel);
	}

private:
	void showImpl(bool update) const {
		nxpl::drawCircle(*lcd, centerX(), centerY(), r, DrawOpt::draw());
	}

	void eraseImpl(bool update) const {
		nxpl::drawCircle(*lcd, centerX(), centerY(), r, DrawOpt::clear());
	}

	void invertImpl(bool update) const {
		nxpl::drawCircle(*lcd, centerX(), centerY(), r, DrawOpt::invert());
	}
};


void NCircle::setPosition(const S8 centerX, const S8 centerY, const S8 radius) {
	if(isVisible()) {
		this->hide();
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

}

#endif /* NCIRCLE_HPP_ */
