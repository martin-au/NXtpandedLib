/*
 * NEllipse.hpp
 *
 *  Created on: 14.08.2014
 *      Author: Martin
 */

#ifndef __NELLIPSE_HPP_
#define __NELLIPSE_HPP_

/** \file
 *	\ingroup NxtLcd
*/

#include "NShape.hpp"
#include "GuiTypes.hpp"

namespace nxpl {

/** \brief Ellipse object for lcd.
 */
class NEllipse : public NShape {
private:
	NPoint center_;
	S8 a_, b_;

	void showShapeImpl() const {
		draw(*lcd, center_, a_, b_, DrawOpt::draw());
	}

	void hideShapeImpl() const {
		draw(*lcd, center_, a_, b_, DrawOpt::clear());
	}

	void invertShapeImpl() const {
		draw(*lcd, center_, a_, b_, DrawOpt::invert());
	}

public:

	/** \brief Draw an ellipse.
	 *
	 * This function lets you draw an ellipse on the screen with its center at the specified x and y location, using the specified radii.
	 * Optionally specify drawing options.
	 * If this argument is not specified it defaults to DrawOpt::draw().
	 * Valid display options are listed in the DrawOpt class.
	 *
	 * @param lcd      The lcd for drawing the ellipse.
	 * @param centerX  The x value for the center of the ellipse.
	 * @param centerY  The y value for the center of the ellipse.
	 * @param a		   The x axis radius.
	 * @param b		   The y axis radius.
	 * @param op       The optional drawing options.
	 */
	static bool draw(NLcd &lcd, NPoint center, S8 a, S8 b, DrawOpt op = DrawOpt::draw());

	inline static bool draw(NPoint center, S8 a, S8 b, DrawOpt op = DrawOpt::draw()) {
		NLcd lcd;
		return draw(lcd, center, a, b, op);
	}

public:

	/** \brief Construct a ellipse object.
	 *
	 * This constructs a ellipse on the given lcd with its center at the specified x and y location, using the specified radius.
	 *
	 * @param nlcd      The lcd for drawing the ellipse.
	 * @param centerX  The x value for the center of the ellipse.
	 * @param centerY  The y value for the center of the ellipse.
	 * @param radius   The radius of the ellipse.
	 */
	NEllipse(NLcd &nlcd, NPoint center, S8 a, S8 b) :
			NShape(nlcd), center_(center), a_(a), b_(b) {
	}

	/** \brief Destructor
	 *  Hides line if visible.
	 */
	virtual ~NEllipse() {
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

	U8 a() const {
		return a_;
	}

	/** \brief Set ellipse radius.
	 *
	 * @param radius New radius.
	 */
	void setA(U8 a) {
		//if(radius <= LCD::WIDTH/2 && radius <= LCD::HEIGHT/2 && r > 0)
		if(a != a_)
			hide();
		a_ = a;
	}

	U8 b() const {
		return b_;
	}

	/** \brief Set ellipse radius.
	 *
	 * @param radius New radius.
	 */
	void setB(U8 b) {
		//if(radius <= LCD::WIDTH/2 && radius <= LCD::HEIGHT/2 && r > 0)
		if(b != b_)
			hide();
		b_ = b;
	}
};


/** \brief Filled ellipse object for lcd.
 */
class NEllipseFilled : public NShape {
private:
	NPoint center_;
	S8 a_, b_;

	void showShapeImpl() const {
		draw(*lcd, center_, a_, b_, DrawOpt::draw());
	}

	void hideShapeImpl() const {
		draw(*lcd, center_, a_, b_, DrawOpt::clear());
	}

	void invertShapeImpl() const {
		draw(*lcd, center_, a_, b_, DrawOpt::invert());
	}

public:

	/** \brief Draw an ellipse.
	 *
	 * This function lets you draw an ellipse on the screen with its center at the specified x and y location, using the specified radii.
	 * Optionally specify drawing options.
	 * If this argument is not specified it defaults to DrawOpt::draw().
	 * Valid display options are listed in the DrawOpt class.
	 *
	 * @param lcd      The lcd for drawing the ellipse.
	 * @param centerX  The x value for the center of the ellipse.
	 * @param centerY  The y value for the center of the ellipse.
	 * @param a		   The x axis radius.
	 * @param b		   The y axis radius.
	 * @param op       The optional drawing options.
	 */
	static bool draw(NLcd &lcd, NPoint center, S8 a, S8 b, DrawOpt op = DrawOpt::draw());

	inline static bool draw(NPoint center, S8 a, S8 b, DrawOpt op = DrawOpt::draw()) {
		NLcd lcd;
		return draw(lcd, center, a, b, op);
	}

public:

	/** \brief Construct a ellipse object.
	 *
	 * This constructs a ellipse on the given lcd with its center at the specified x and y location, using the specified radius.
	 *
	 * @param nlcd      The lcd for drawing the ellipse.
	 * @param centerX  The x value for the center of the ellipse.
	 * @param centerY  The y value for the center of the ellipse.
	 * @param radius   The radius of the ellipse.
	 */
	NEllipseFilled(NLcd &nlcd, NPoint center, S8 a, S8 b) :
			NShape(nlcd), center_(center), a_(a), b_(b) {
	}

	/** \brief Destructor
	 *  Hides line if visible.
	 */
	virtual ~NEllipseFilled() {
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

	U8 a() const {
		return a_;
	}

	/** \brief Set ellipse radius.
	 *
	 * @param radius New radius.
	 */
	void setA(U8 a) {
		//if(radius <= LCD::WIDTH/2 && radius <= LCD::HEIGHT/2 && r > 0)
		if(a != a_)
			hide();
		a_ = a;
	}

	U8 b() const {
		return b_;
	}

	/** \brief Set ellipse radius.
	 *
	 * @param radius New radius.
	 */
	void setB(U8 b) {
		//if(radius <= LCD::WIDTH/2 && radius <= LCD::HEIGHT/2 && r > 0)
		if(b != b_)
			hide();
		b_ = b;
	}
};

}

#endif /* NCIRCLE_HPP_ */
