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
#include "NPixelBox.hpp"
#include "GuiTypes.hpp"

namespace nxpl {

/**
 * \brief Rectangle object for lcd.
 */
class NRectangle : public NShape {
private:
	NPixelBox geometry_;

	void showShapeImpl() const {
		draw(*lcd, geometry_, DrawOpt::draw());
	}

	void hideShapeImpl() const {
		draw(*lcd, geometry_, DrawOpt::clear());
	}

	void invertShapeImpl() const {
		draw(*lcd, geometry_, DrawOpt::invert());
	}

public:

	/**
	 * \brief Draw a rectangle.
	 *
	 * This function lets you draw a rectangle on the given lcd at x, y with the
	 * specified width and height.
	 * Optionally specify drawing options.
	 * If this argument is not specified it defaults to DrawOpt::draw().
	 * Valid display options are listed in the DrawOpt class.
	 *
	 * @param lcd  The lcd for drawing the rectangle.
	 * @param x0   The x value for the top left corner of the rectangle.
	 * @param y0   The y value for the top left corner of the rectangle.
	 * @param w    The width of the rectangle.
	 * @param h    The height of the rectangle.
	 * @param op   The optional drawing options.
	 */
	static bool draw(NLcd &lcd, NPixelBox geometry, DrawOpt op = DrawOpt::draw());

	static inline bool draw(NPixelBox geometry, DrawOpt op = DrawOpt::draw()) {
		NLcd lcd;
		return draw(lcd, geometry, op);
	}

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
	NRectangle(NLcd &nlcd, const NPixelBox geometry) :
			NShape(nlcd), geometry_(geometry) {
	}

	/** \brief Destructor
	 *  Hides rectangle if visible.
	 */
	virtual ~NRectangle() {
		if(isVisible()) {
			this->hide();
		}
	}

	void setGeometry(const NPixelBox &geometry) {
		this->hide();
		geometry_ = geometry;
	}

	NPixelBox geometry() const {
		return geometry_;
	}
};


/**
 * \brief Rectangle object for lcd.
 */
class NRectangleFilled : public NShape {
private:
	NPixelBox geometry_;

	void showShapeImpl() const {
		draw(*lcd, geometry_, DrawOpt::draw());
	}

	void hideShapeImpl() const {
		draw(*lcd, geometry_, DrawOpt::clear());
	}

	void invertShapeImpl() const {
		draw(*lcd, geometry_, DrawOpt::invert());
	}

public:
	/**
	 * \brief Draw a filled rectangle.
	 *
	 * This function lets you draw a filled rectangle on the given lcd at x, y with the
	 * specified width and height.
	 * Optionally specify drawing options.
	 * If this argument is not specified it defaults to DrawOpt::draw().
	 * Valid display options are listed in the DrawOpt class.
	 *
	 * @param lcd  The lcd for drawing the rectangle.
	 * @param x0   The x value for the top left corner of the rectangle.
	 * @param y0   The y value for the top left corner of the rectangle.
	 * @param w    The width of the rectangle.
	 * @param h    The height of the rectangle.
	 * @param op   The optional drawing options.
	 */
	static bool draw(NLcd &lcd, const NPixelBox &geometry, DrawOpt op = DrawOpt::draw());

	static inline bool draw(const NPixelBox &geometry, DrawOpt op = DrawOpt::draw()) {
		NLcd lcd;
		return draw(lcd, geometry, op);
	}

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
	NRectangleFilled(NLcd &nlcd, const NPixelBox geometry) :
			NShape(nlcd), geometry_(geometry) {
	}

	/** \brief Destructor
	 *  Hides rectangle if visible.
	 */
	virtual ~NRectangleFilled() {
		if(isVisible()) {
			this->hide();
		}
	}

	void setGeometry(const NPixelBox &geometry) {
		this->hide();
		geometry_ = geometry;
	}

	NPixelBox geometry() const {
		return geometry_;
	}
};


}

#endif /* __NREACTANGLE_HPP_ */
