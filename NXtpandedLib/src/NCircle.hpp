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

/** \brief Drawable circle object.
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
	 * This function lets you draw a circle on the given lcd with its center at the specified point, using the specified radius.
	 * Optionally specify drawing options.
	 * If this argument is not specified it defaults to DrawOpt::draw().
	 * Valid display options are listed in the DrawOpt class.
	 *
	 * @param lcd      The lcd for drawing the circle.
	 * @param center   The center point of the circle.
	 * @param radius   The radius of the circle.
	 * @param op       	The optional drawing options.
	 */
	bool static draw(NLcd &lcd, NPoint center, S8 radius, DrawOpt op = DrawOpt::draw());

	/** \brief Draw a circle directly on nxt lcd.
	 *
	 * This function lets you draw a circle with its center at the specified point location, using the specified radius.
	 * Optionally specify drawing options.
	 * If this argument is not specified it defaults to DrawOpt::draw().
	 * Valid display options are listed in the DrawOpt class.
	 *
	 * @param center   The center point of the circle.
	 * @param radius   The radius of the circle.
	 * @param op       	The optional drawing options.
	 */
	bool static inline draw(NPoint center, S8 radius, DrawOpt op = DrawOpt::draw()) {
		NLcd lcd;
		return draw(lcd, center, radius, op);
	}

public:

	/** \brief Construct a circle object.
	 *
	 * This constructs a circle on the given lcd with its center at the specified point, using the specified radius.
	 *
	 * @param nlcd     The lcd for drawing the circle.
	 * @param center   The center point of the circle.
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

	/** \brief Get the center point.
	 */
	NPoint center() const {
		return center_;
	}

	/**
	 * \brief Set center point.
	 */
	void setCenter(NPoint center) {
		bool visible = isVisible();
		if(center != center_)
			hide();
		center_ = center;
		if(visible)
			show();
	}

	/**
	 * \brief Set center X-Coordinate.
	 */
	void setCenterX(S16 x) {
		bool visible = isVisible();
		hide();
		center_.setX(x);
		if(visible)
			show();
	}

	/**
	 * \brief Set center Y-Coordinate.
	 */
	void setCenterY(S16 y) {
		bool visible = isVisible();
		hide();
		center_.setY(y);
		if(visible)
			show();
	}

	/* \brief Get radius **/
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


/** \brief Drawable filled circle object.
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
	 * This function lets you draw a filled circle on the given lcd with its center at the specified point, using the specified radius.
	 * Optionally specify drawing options.
	 * If this argument is not specified it defaults to DrawOpt::draw().
	 * Valid display options are listed in the DrawOpt class.
	 *
	 * @param lcd      The lcd for drawing the circle.
	 * @param center   The center point of the circle.
	 * @param radius   The radius of the circle.
	 * @param op       The optional drawing options.
	 */
	static bool draw(NLcd &lcd, NPoint center, S8 radius, DrawOpt op = DrawOpt::draw());

	/** \brief Draw a filled circle directly on nxt lcd.
	 *
	 * This function lets you draw a filled circle with its center at the specified point location, using the specified radius.
	 * Optionally specify drawing options.
	 * If this argument is not specified it defaults to DrawOpt::draw().
	 * Valid display options are listed in the DrawOpt class.
	 *
	 * @param center   The center point of the circle.
	 * @param radius   The radius of the circle.
	 * @param op       	The optional drawing options.
	 */
	static inline bool draw(NPoint center, S8 radius, DrawOpt op = DrawOpt::draw()) {
		NLcd lcd;
		return draw(lcd, center, radius, op);
	}


public:

	/** \brief Construct a filled circle object.
	 *
	 * This constructs a filled circle on the given lcd with its center at the specified point, using the specified radius.
	 *
	 * @param nlcd     The lcd for drawing the circle.
	 * @param center   The center point of the circle.
	 * @param radius   The radius of the circle.
	 */
	NCircleFilled(NLcd &nlcd, NPoint center, U8 radius) :
			NShape(nlcd), center_(center), r(radius) {
	}

	/** \brief Destructor
	 *  Hides circle if visible.
	 */
	virtual ~NCircleFilled() {
		hide();
	}

	/** \brief Get the center point.
	 */
	NPoint center() const {
		return center_;
	}

	/**
	 * \brief Set center point.
	 */
	void setCenter(NPoint center) {
		bool visible = isVisible();
		if(center != center_)
			hide();
		center_ = center;
		if(visible)
			show();
	}

	/**
	 * \brief Set center X-Coordinate.
	 */
	void setCenterX(S16 x) {
		bool visible = isVisible();
		hide();
		center_.setX(x);
		if(visible)
			show();
	}

	/**
	 * \brief Set center Y-Coordinate.
	 */
	void setCenterY(S16 y) {
		bool visible = isVisible();
		hide();
		center_.setY(y);
		if(visible)
			show();
	}

	/* \brief Get radius **/
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
