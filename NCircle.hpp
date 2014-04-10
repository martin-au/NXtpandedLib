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
	NPoint center_;
	U8 r;

	void showShapeImpl() const {
		nxpl::drawCircle(lcd, center_, r, DrawOpt::draw());
	}

	void hideShapeImpl() const {
		nxpl::drawCircle(lcd, center_, r, DrawOpt::clear());
	}

	void invertShapeImpl() const {
		nxpl::drawCircle(lcd, center_, r, DrawOpt::invert());
	}
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
	NCircle(NLcd &nlcd, NPoint center, U8 radius) :
			NShape(nlcd), center_(center), r(radius) {
	}

	/** \brief Destructor
	 *  Hides line if visible.
	 */
	virtual ~NCircle() {
		hide();
	}

	/** \brief Calculate the x-coordinate of the center point.
	 *
	 * @return Center point x-coordinate
	 */
	NPoint center() const {
		return center_;
	}

	void setCenter(NPoint center) {
		if(center != center_)
			hide();
		center_ = center;
	}

	U8 radius() const {
		return r;
	}

	/** \brief Set circle radius.
	 *
	 * @param radius New radius.
	 */
	void setRadius(U8 radius) {
		//if(radius <= LCD::WIDTH/2 && radius <= LCD::HEIGHT/2 && r > 0)
		if(radius != r)
			hide();
		r = radius;
	}
};


/** \brief Circle object for lcd.
 */
class NCircleFilled : public NShape {
private:
	NPoint center_;
	U8 r;

	void showShapeImpl() const {
		drawCircleFilled(lcd, center_, r, DrawOpt::draw());
	}

	void hideShapeImpl() const {
		drawCircleFilled(lcd, center_, r, DrawOpt::clear());
	}

	void invertShapeImpl() const {
		drawCircleFilled(lcd, center_, r, DrawOpt::invert());
	}
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
	NCircleFilled(NLcd &nlcd, NPoint center, U8 radius) :
			NShape(nlcd), center_(center), r(radius) {
	}

	/** \brief Destructor
	 *  Hides line if visible.
	 */
	virtual ~NCircleFilled() {
		hide();
	}

	/** \brief Calculate the x-coordinate of the center point.
	 *
	 * @return Center point x-coordinate
	 */
	NPoint center() const {
		return center_;
	}

	void setCenter(NPoint center) {
		if(center != center_)
			hide();
		center_ = center;
	}

	U8 radius() const {
		return r;
	}

	/** \brief Set circle radius.
	 *
	 * @param radius New radius.
	 */
	void setRadius(U8 radius) {
		//if(radius <= LCD::WIDTH/2 && radius <= LCD::HEIGHT/2 && r > 0)
		if(radius != r)
			hide();
		r = radius;
	}
};

}

#endif /* NCIRCLE_HPP_ */
