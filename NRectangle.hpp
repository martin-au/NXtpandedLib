/*
 * NReactangle.hpp
 *
 *  Created on: 01.12.2013
 *      Author: Martin
 */

#ifndef __RECTANGLE_HPP_
#define __RECTANGLE_HPP_

/** \file
 *	\ingroup NxtLcd
*/

#include "NShape.hpp"
#include "LcdDrawer.hpp"

namespace nxpl {

/**
 * \brief Rectangle object for lcd.
 */
class NRectangle : public NShape {
private:

public:

	/**
	 * \brief Construct rectangle object on given lcd.
	 *
	 * Constructs a rectangle on the given lcd at x, y with the
	 * specified width and height.
	 *
	 * @param nlcd  The lcd for drawing the rectangle.
	 * @param x0   The x value for the top left corner of the rectangle.
	 * @param y0   The y value for the top left corner of the rectangle.
	 * @param w    The width of the rectangle.
	 * @param h    The height of the rectangle.
	 */
	NRectangle(NLcd *nlcd, S8 x0, S8 y0, S8 w, S8 h) :
			NShape(nlcd) {
		setPixelField(x0, y0, w, h);
	}

	/** \brief Destructor
	 *  Hides rectangle if visible.
	 */
	virtual ~NRectangle() {
		if(isVisible()) {
			NRectangle::hide();
		}
	}

	/** \brief Set position and size of rectangle.
	 *
	 * Use NWidget::keep to keep the actual value.
	 *
	 * @param x0 The x value for the top left corner of the rectangle.
	 * @param y0 The y value for the top left corner of the rectangle.
	 * @param w  The width of the rectangle.
	 * @param h  The height of the rectangle.
	 */
	void setPosition(S8 x0 = keep, S8 y0 = keep, S8 w = keep, S8 h = keep);

	/** \brief Fill the rectangle.
	 */
	void fill() const {
		if (lcd == 0) {
			return;
		}
		nxpl::drawRectangleFilled(*lcd, x(), y(), width(), height(),
				DrawOpt::draw());
	}

	/** \brief Erase the fill of the rectangle.
	 */
	void fillErase() const {
		if (lcd == 0) {
			return;
		}
		nxpl::drawRectangleFilled(*lcd, x(), y(), width(), height(),
				DrawOpt::clear());
	}

	/** \brief Invert the fill of the rectangle.
	 */
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
