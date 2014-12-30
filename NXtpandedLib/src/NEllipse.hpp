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
#include "NLcd.hpp"

namespace nxpl {

/** \brief Ellipse object for lcd.
 */
class NEllipse : public NShape {
private:
	NPoint center_;
	S8 a_, b_;

	void showShapeImpl() const {
		draw(pixelMatrix, center_, a_, b_, DrawOpt::draw());
	}

	void hideShapeImpl() const {
		draw(pixelMatrix, center_, a_, b_, DrawOpt::clear());
	}

	void invertShapeImpl() const {
		draw(pixelMatrix, center_, a_, b_, DrawOpt::invert());
	}

public:

	/** \brief Draw an ellipse.
	 *
	 * This function lets you draw an ellipse on given pixel matrix with its center at the specified point, using the specified radii.
	 * Optionally specify drawing options.
	 * If this argument is not specified it defaults to DrawOpt::draw().
	 * Valid display options are listed in the DrawOpt class.
	 *
	 * @param matrix   The pixel matrix for drawing the ellipse.
	 * @param center   The center of the ellipse.
	 * @param a		   The x axis radius.
	 * @param b		   The y axis radius.
	 * @param op       The optional drawing options.
	 */
	static bool draw(NGenericPixelMatrix *matrix, NPoint center, S8 a, S8 b, DrawOpt op = DrawOpt::draw());

	/** \brief Draw an ellipse directly on nxt lcd.
	 *
	 * This function lets you draw an ellipse on nxt lcd with its center at the specified point, using the specified radii.
	 * Optionally specify drawing options.
	 * If this argument is not specified it defaults to DrawOpt::draw().
	 * Valid display options are listed in the DrawOpt class.
	 *
	 * @param center   The center of the ellipse.
	 * @param a		   The x axis radius.
	 * @param b		   The y axis radius.
	 * @param op       The optional drawing options.
	 */
	inline static bool draw(NPoint center, S8 a, S8 b, DrawOpt op = DrawOpt::draw()) {
		NLcd lcd;
		return draw(&lcd, center, a, b, op);
	}

public:

	/** \brief Construct a ellipse object.
	 *
	 * This constructs a ellipse on the given pixel matrix with its center at the specified point, using the specified radius.
	 *
	 * @param matrix   The pixel matrix for drawing the ellipse.
	 * @param center   The center of the ellipse.
	 * @param a		   The x axis radius.
	 * @param b		   The y axis radius.
	 */
	NEllipse(NGenericPixelMatrix *matrix, NPoint center, S8 a, S8 b) :
			NShape(matrix), center_(center), a_(a), b_(b) {
	}

	/** \brief Destructor
	 *  Hides line if visible.
	 */
	virtual ~NEllipse() {
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

	/* \brief Get radius a **/
	U8 a() const {
		return a_;
	}

	/** \brief Set ellipse radius.
	 *
	 * @param radius New radius.
	 */
	void setA(U8 a) {
		bool visible = isVisible();
		if(a != a_)
			hide();
		a_ = a;
		if(visible)
			show();
	}

	/* \brief Get radius b**/
	U8 b() const {
		return b_;
	}

	/** \brief Set ellipse radius.
	 *
	 * @param radius New radius.
	 */
	void setB(U8 b) {
		bool visible = isVisible();
		//if(radius <= LCD::WIDTH/2 && radius <= LCD::HEIGHT/2 && r > 0)
		if(b != b_)
			hide();
		b_ = b;
		if(visible)
			show();
	}
};


/** \brief Filled ellipse object for lcd.
 */
class NEllipseFilled : public NShape {
private:
	NPoint center_;
	S8 a_, b_;

	void showShapeImpl() const {
		draw(pixelMatrix, center_, a_, b_, DrawOpt::draw());
	}

	void hideShapeImpl() const {
		draw(pixelMatrix, center_, a_, b_, DrawOpt::clear());
	}

	void invertShapeImpl() const {
		draw(pixelMatrix, center_, a_, b_, DrawOpt::invert());
	}

public:

	/** \brief Draw an filled ellipse.
	 *
	 * This function lets you draw an filled ellipse on given pixel matrix with its center at the specified point, using the specified radii.
	 * Optionally specify drawing options.
	 * If this argument is not specified it defaults to DrawOpt::draw().
	 * Valid display options are listed in the DrawOpt class.
	 *
	 * @param matrix   The pixel matrix for drawing the ellipse.
	 * @param center   The center of the ellipse.
	 * @param a		   The x axis radius.
	 * @param b		   The y axis radius.
	 * @param op       The optional drawing options.
	 */
	static bool draw(NGenericPixelMatrix *matrix, NPoint center, S8 a, S8 b, DrawOpt op = DrawOpt::draw());

	/** \brief Draw an filled ellipse directly on nxt lcd.
	 *
	 * This function lets you draw an filled ellipse on nxt lcd with its center at the specified point, using the specified radii.
	 * Optionally specify drawing options.
	 * If this argument is not specified it defaults to DrawOpt::draw().
	 * Valid display options are listed in the DrawOpt class.
	 *
	 * @param center   The center of the ellipse.
	 * @param a		   The x axis radius.
	 * @param b		   The y axis radius.
	 * @param op       The optional drawing options.
	 */
	inline static bool draw(NPoint center, S8 a, S8 b, DrawOpt op = DrawOpt::draw()) {
		NLcd lcd;
		return draw(&lcd, center, a, b, op);
	}

public:

	/** \brief Construct filled ellipse object.
	 *
	 * This constructs a ellipse on the given pixel matrix with its center at the specified point, using the specified radius.
	 *
	 * @param matrix   The pixel matrix for drawing the ellipse.
	 * @param center   The center of the ellipse.
	 * @param a		   The x axis radius.
	 * @param b		   The y axis radius.
	 */
	NEllipseFilled(NGenericPixelMatrix *matrix, NPoint center, S8 a, S8 b) :
			NShape(matrix), center_(center), a_(a), b_(b) {
	}

	/** \brief Destructor
	 *  Hides line if visible.
	 */
	virtual ~NEllipseFilled() {
		hide();
	}

	/** \brief Get the center point.
	 */
	NPoint center() const
	{
		return center_;
	}

	/**
	 * \brief Set center point.
	 */
	void setCenter(NPoint center)
	{
		bool visible = isVisible();
		if (center != center_)
			hide();
		center_ = center;
		if (visible)
			show();
	}

	/**
	 * \brief Set center X-Coordinate.
	 */
	void setCenterX(S16 x)
	{
		bool visible = isVisible();
		hide();
		center_.setX(x);
		if (visible)
			show();
	}

	/**
	 * \brief Set center Y-Coordinate.
	 */
	void setCenterY(S16 y)
	{
		bool visible = isVisible();
		hide();
		center_.setY(y);
		if (visible)
			show();
	}

	/* \brief Get radius a **/
	U8 a() const
	{
		return a_;
	}

	/** \brief Set ellipse radius.
	 *
	 * @param radius New radius.
	 */
	void setA(U8 a)
	{
		bool visible = isVisible();
		if (a != a_)
			hide();
		a_ = a;
		if (visible)
			show();
	}

	/* \brief Get radius b**/
	U8 b() const
	{
		return b_;
	}

	/** \brief Set ellipse radius.
	 *
	 * @param radius New radius.
	 */
	void setB(U8 b)
	{
		bool visible = isVisible();
		//if(radius <= LCD::WIDTH/2 && radius <= LCD::HEIGHT/2 && r > 0)
		if (b != b_)
			hide();
		b_ = b;
		if (visible)
			show();
	}
};

}

#endif /* NCIRCLE_HPP_ */
