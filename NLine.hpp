/*
 * NLine.hpp
 *
 *  Created on: 23.11.2013
 *      Author: Martin
 */

#define __NLINE_HPP_

#ifndef __NLINE_HPP_
#define __NLINE_HPP_

/** \file
 *	\ingroup NxtLcd
*/

#include "NShape.hpp"
#include "LcdDrawer.hpp"

namespace nxpl {

/**
 * \brief Line object for lcd.
 */
class NLine : public NShape {
private:

public:
	/**
	 * \brief Construct line object on given lcd.
	 *
	 * Line object from x0, y0 to x1, y1.
	 *
	 * @param nlcd  The lcd for drawing the line.
	 * @param x0   The x value for the start of the line.
	 * @param y0   The y value for the start of the line.
	 * @param x1   The x value for the end of the line.
	 * @param y1   The y value for the end of the line.
	 */
	explicit NLine(NLcd *nlcd, S8 x0, S8 y0, S8 x1, S8 y1) :
			NShape(nlcd) {
	}
	/**
	 * \brief Basic line constructor.
	 */
	NLine();

	/** \brief Destructor
	 *  Hides line if visible.
	 */
	virtual ~NLine() {
		if(isVisible()) {
			NLine::hide();
		}
	}

	/** \brief Set positions of start/end -points of line.
	 *
	 * Use NWidget::keep to keep the actual value.
	 *
	 * @param x0   The x value for the start of the line.
	 * @param y0   The y value for the start of the line.
	 * @param x1   The x value for the end of the line.
	 * @param y1   The y value for the end of the line.
	 */
	void setPosition(S8 x0 = keep, S8 y0 = keep, S8 x1 = keep, S8 y1 = keep);

	/** \brief Calculates the x-coordinate of end point.
	 * @return x-coordinate of end point.
	 */
	S8 endX() const {
		return x() + width();
	}

	/** \brief Calculates the y-coordinate of end point.
	 * @return y-coordinate of end point.
	 */
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
