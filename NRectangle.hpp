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
#include "NPixelBox.hpp"

namespace nxpl {

/**
 * \brief Rectangle object for lcd.
 */
class NRectangle : public NShape {
private:
	NPixelBox geometry_;

	void showShapeImpl() const {
		nxpl::drawRectangle(*lcd, geometry_, DrawOpt::draw());
	}

	void hideShapeImpl() const {
		nxpl::drawRectangle(*lcd, geometry_, DrawOpt::clear());
	}

	void invertShapeImpl() const {
		nxpl::drawRectangle(*lcd, geometry_, DrawOpt::invert());
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
		nxpl::drawRectangleFilled(*lcd, geometry_, DrawOpt::draw());
	}

	void hideShapeImpl() const {
		nxpl::drawRectangleFilled(*lcd, geometry_, DrawOpt::clear());
	}

	void invertShapeImpl() const {
		nxpl::drawRectangleFilled(*lcd, geometry_, DrawOpt::invert());
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
