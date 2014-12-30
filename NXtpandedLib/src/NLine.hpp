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
#include "NLcd.hpp"

namespace nxpl {

/**
 * \brief Line object for lcd.
 */
class NLine : public NShape {
private:
	NPoint start_, end_;

	void showShapeImpl() const {
		draw(pixelMatrix, start_, end_, DrawOpt::draw());
	}


	void hideShapeImpl() const {
		draw(pixelMatrix, start_, end_, DrawOpt::clear());
	}


	void invertShapeImpl() const {
		draw(pixelMatrix, start_, end_, DrawOpt::invert());
	}

public:

	/**
	 * \brief Draw a line on given pixel matrix.
	 *
	 * This function lets you draw a line on the given pixel matrix from start point to end point.
	 * Optionally specify drawing options.
	 * If this argument is not specified it defaults to DrawOpt::draw().
	 * Valid display options are listed in the DrawOpt class.
	 *
	 * @param matrix  The generic pixel matrix for drawing the line.
	 * @param start   The start point for the of the line.
	 * @param end     The end point of the line.
	 * @param op      The optional drawing options.
	 */
	static bool draw(NGenericPixelMatrix *matrix, NPoint start, NPoint end, DrawOpt op = DrawOpt::draw());

	/**
	 * \brief Draw a line directly on nxt lcd.
	 *
	 * This function lets you draw a line on the nxt lcd from start point to end point.
	 * Optionally specify drawing options.
	 * If this argument is not specified it defaults to DrawOpt::draw().
	 * Valid display options are listed in the DrawOpt class.
	 *
	 * @param start   The start point for the of the line.
	 * @param end     The end point of the line.
	 * @param op      The optional drawing options.
	 */
	static inline bool draw(NPoint start, NPoint end, DrawOpt op = DrawOpt::draw()) {
		NLcd lcd;
		return draw(&lcd, start, end, op);
	}

public:

	/**
	 * \brief Construct line object on given pixel matrix.
	 *
	 * Creates a line object on given pixel matrix from start point to end point.
	 *
	 * @param matrix  The pixel matrix for drawing the line.
	 * @param start   The start point for the of the line.
	 * @param end     The end point of the line.
	 */
	explicit NLine(NGenericPixelMatrix *matrix, NPoint startP, NPoint endP) :
			NShape(matrix), start_(startP), end_(endP) {
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

	/** \brief Set position of start point.
	 *
     * @param startP  The start point for the of the line.
	 */
	void setStart(NPoint startP) {
		bool visible = isVisible();
		if(startP != start_)
			this->hide();
		start_ = startP;
		if(visible)
			show();
	}

	/** \brief Set position of start point X-Coordinate.
	 *
     * @param x  X-Coordinate of start point.
	 */
	void setStartX(S16 x) {
		bool visible = isVisible();
		start_.setX(x);
		if(visible)
			show();
	}

	/** \brief Set position of start point Y-Coordinate.
	 *
     * @param x  Y-Coordinate of start point.
	 */
	void setStartY(S16 y) {
		bool visible = isVisible();
		start_.setY(y);
		if(visible)
			show();
	}

	/** \brief Set position of end point.
	 *
     * @param endP  The end point for the of the line.
	 */
	void setEnd(NPoint endP) {
		bool visible = isVisible();
		if(endP != end_)
			this->hide();
		end_ = endP;
		if(visible)
			show();
	}

	/** \brief Set position of end point X-Coordinate.
	 *
     * @param x  X-Coordinate of end point.
	 */
	void setEndX(S16 x) {
		bool visible = isVisible();
		end_.setX(x);
		if(visible)
			show();
	}

	/** \brief Set position of end point Y-Coordinate.
	 *
     * @param x  Y-Coordinate of end point.
	 */
	void setEndY(S16 y) {
		bool visible = isVisible();
		end_.setY(y);
		if(visible)
			show();
	}

	/** \brief Get the end point.
	 * @return End point.
	 */
	NPoint end() const {
		return end_;
	}

	/** \brief Get the start point.
	 * @return Start point.
	 */
	NPoint start() const {
		return start_;
	}
};

}

#endif /* NLINE_HPP_ */
