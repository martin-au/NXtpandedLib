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
#include "GuiTypes.hpp"


namespace nxpl {

/** \brief Circle object for lcd.
 */
class NCircle : public NShape {
private:
	NPoint center_;
	U8 r;

	void showShapeImpl() const {
		draw(*lcd, center_, r, DrawOpt::draw());
	}

	void hideShapeImpl() const {
		draw(*lcd, center_, r, DrawOpt::clear());
	}

	void invertShapeImpl() const {
		draw(*lcd, center_, r, DrawOpt::invert());
	}

public:

	/** \brief Draw a circle.
	 *
	 * This function lets you draw a circle on the given lcd with its center at the specified x and y location, using the specified radius.
	 * Optionally specify drawing options.
	 * If this argument is not specified it defaults to DrawOpt::draw().
	 * Valid display options are listed in the DrawOpt class.
	 *
	 * @param lcd      The lcd for drawing the circle.
	 * @param centerX  The x value for the center of the circle.
	 * @param centerY  The y value for the center of the circle.
	 * @param radius   The radius of the circle.
	 * @param op       	The optional drawing options.
	 */
	bool static draw(NLcd &lcd, NPoint center, S8 radius, DrawOpt op = DrawOpt::draw());

	bool static inline draw(NPoint center, S8 radius, DrawOpt op = DrawOpt::draw()) {
		NLcd lcd;
		return draw(lcd, center, radius, op);
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
		bool visible = isVisible();
		if(center != center_)
			hide();
		center_ = center;
		if(visible)
			show();
	}

	void setCenterX(S16 x) {
		bool visible = isVisible();
		hide();
		center_.setX(x);
		if(visible)
			show();
	}

	void setCenterY(S16 y) {
		bool visible = isVisible();
		hide();
		center_.setY(y);
		if(visible)
			show();
	}

	U8 radius() const {
		return r;
	}

	/** \brief Set circle radius.
	 *
	 * @param radius New radius.
	 */
	void setRadius(U8 radius) {
		bool visible = isVisible();
		if(radius != r)
			hide();
		r = radius;
		if(visible)
			show();
	}
};


/** \brief Circle object for lcd.
 */
class NCircleFilled : public NShape {
private:
	NPoint center_;
	U8 r;

	void showShapeImpl() const {
		draw(*lcd, center_, r, DrawOpt::draw());
	}

	void hideShapeImpl() const {
		draw(*lcd, center_, r, DrawOpt::clear());
	}

	void invertShapeImpl() const {
		draw(*lcd, center_, r, DrawOpt::invert());
	}

public:

	/** \brief Draw a filled circle.
	 *
	 * This function lets you draw a filled circle on the given lcd with its center at the specified x and y location, using the specified radius.
	 * Optionally specify drawing options.
	 * If this argument is not specified it defaults to DrawOpt::draw().
	 * Valid display options are listed in the DrawOpt class.
	 *
	 * @param lcd      The lcd for drawing the circle.
	 * @param centerX  The x value for the center of the circle.
	 * @param centerY  The y value for the center of the circle.
	 * @param radius   The radius of the circle.
	 * @param op       	The optional drawing options.
	 */
	static bool draw(NLcd &lcd, NPoint center, S8 radius, DrawOpt op = DrawOpt::draw());

	static inline bool draw(NPoint center, S8 radius, DrawOpt op = DrawOpt::draw()) {
		NLcd lcd;
		return draw(lcd, center, radius, op);
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
		bool visible = isVisible();
		if(center != center_)
			hide();
		center_ = center;
		if(visible)
			show();
	}

	void setCenterX(S16 x) {
		bool visible = isVisible();
		hide();
		center_.setX(x);
		if(visible)
			show();
	}

	void setCenterY(S16 y) {
		bool visible = isVisible();
		hide();
		center_.setY(y);
		if(visible)
			show();
	}

	U8 radius() const {
		return r;
	}

	/** \brief Set circle radius.
	 *
	 * @param radius New radius.
	 */
	void setRadius(U8 radius) {
		bool visible = isVisible();
		if(radius != r)
			hide();
		r = radius;
		if(visible)
			show();

	}
};

}

#endif /* NCIRCLE_HPP_ */
