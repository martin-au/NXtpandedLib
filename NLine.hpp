/*
 * NLine.hpp
 *
 *  Created on: 23.11.2013
 *      Author: Martin
 */


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
	NPoint start_, end_;
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
	explicit NLine(NLcd &nlcd, NPoint startP, NPoint endP) :
			NShape(nlcd), start_(startP), end_(endP) {
	}
	/**
	 * \brief Basic line constructor.
	 */
	NLine();

	/** \brief Destructor
	 *  Hides line if visible.
	 */
	virtual ~NLine() {
		this->hide();
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
	//void setPosition(S8 x0 = keep, S8 y0 = keep, S8 x1 = keep, S8 y1 = keep);

	void setStart(NPoint startP) {
		if(startP != start_)
			this->hide();
		start_ = startP;
	}

	void setEnd(NPoint endP) {
		if(endP != end_)
			this->hide();
		end_ = endP;
	}

	/** \brief Calculates the x-coordinate of end point.
	 * @return x-coordinate of end point.
	 */
	NPoint end() const {
		return end_;
	}

	/** \brief Calculates the y-coordinate of end point.
	 * @return y-coordinate of end point.
	 */
	NPoint start() const {
		return start_;
	}

private:
	void showImpl(bool update) const {
		nxpl::drawLine(*lcd, start_, end_, DrawOpt::draw());
	}


	void eraseImpl(bool update) const {
		nxpl::drawLine(*lcd, start_, end_, DrawOpt::clear());
	}


	void invertImpl(bool update) const {
		nxpl::drawLine(*lcd, start_, end_, DrawOpt::invert());
	}
};

}

#endif /* NLINE_HPP_ */
