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
#include "GuiTypes.hpp"

namespace nxpl {

/**
 * \brief Line object for lcd.
 */
class NLine : public NShape {
private:
	NPoint start_, end_;

	void showShapeImpl() const {
		draw(*lcd, start_, end_, DrawOpt::draw());
	}


	void hideShapeImpl() const {
		draw(*lcd, start_, end_, DrawOpt::clear());
	}


	void invertShapeImpl() const {
		draw(*lcd, start_, end_, DrawOpt::invert());
	}

public:
	/**
	 * \brief Draw a line.
	 *
	 * This function lets you draw a line on the given lcd from x0, y0 to x1, y1.
	 * Optionally specify drawing options.
	 * If this argument is not specified it defaults to DrawOpt::draw().
	 * Valid display options are listed in the DrawOpt class.
	 *
	 * @param x0   The x value for the start of the line.
	 * @param y0   The y value for the start of the line.
	 * @param x1   The x value for the end of the line.
	 * @param y1   The y value for the end of the line.
	 * @param op   The optional drawing options.
	 * @param lcd  The lcd for drawing the line.
	 */
	static bool draw(NLcd &lcd, NPoint start, NPoint end, DrawOpt op = DrawOpt::draw());

	static inline bool draw(NPoint start, NPoint end, DrawOpt op = DrawOpt::draw()) {
		NLcd lcd;
		return draw(lcd, start, end, op);
	}

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
		bool visible = isVisible();
		if(startP != start_)
			this->hide();
		start_ = startP;
		if(visible)
			show();
	}

	void setStartX(S16 x) {
		bool visible = isVisible();
		start_.setX(x);
		if(visible)
			show();
	}

	void setStartY(S16 y) {
		bool visible = isVisible();
		start_.setY(y);
		if(visible)
			show();
	}

	void setEnd(NPoint endP) {
		bool visible = isVisible();
		if(endP != end_)
			this->hide();
		end_ = endP;
		if(visible)
			show();
	}

	void setEndX(S16 x) {
		bool visible = isVisible();
		end_.setX(x);
		if(visible)
			show();
	}

	void setEndY(S16 y) {
		bool visible = isVisible();
		end_.setY(y);
		if(visible)
			show();
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
};

}

#endif /* NLINE_HPP_ */
