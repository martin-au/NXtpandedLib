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
#include "NLcd.hpp"

namespace nxpl {

/**
 * \brief Rectangle object for lcd.
 */
class NRectangle : public NShape {
private:
	NPixelBox geometry_;

	void showShapeImpl() const {
		draw(pixelMatrix, geometry_, DrawOpt::draw());
	}

	void hideShapeImpl() const {
		draw(pixelMatrix, geometry_, DrawOpt::clear());
	}

	void invertShapeImpl() const {
		draw(pixelMatrix, geometry_, DrawOpt::invert());
	}

public:

	/**
	 * \brief Draw a rectangle on given pixel matrix.
	 *
	 * This function lets you draw a rectangle on the given pixel matrix and geometry.
	 * Optionally specify drawing options.
	 * If this argument is not specified it defaults to DrawOpt::draw().
	 * Valid display options are listed in the DrawOpt class.
	 *
	 * @param matrix  	 The pixel matrix for drawing the rectangle.
	 * @param geometry   The geometry of the rectangle.
	 * @param op   		 The optional drawing options.
	 */
	static bool draw(NGenericPixelMatrix *matrix, NPixelBox geometry, DrawOpt op = DrawOpt::draw());

	/**
	 * \brief Draw a rectangle directly on nxt lcd.
	 *
	 * This function lets you draw a rectangle directly on nxt lcd with given geometry.
	 * Optionally specify drawing options.
	 * If this argument is not specified it defaults to DrawOpt::draw().
	 * Valid display options are listed in the DrawOpt class.
	 *
	 * @param geometry   The geometry of the rectangle.
	 * @param op   		 The optional drawing options.
	 */
	static inline bool draw(NPixelBox geometry, DrawOpt op = DrawOpt::draw()) {
		NLcd lcd;
		return draw(&lcd, geometry, op);
	}

public:

	/**
	 * \brief Construct rectangle object on given pixel matrix.
	 *
	 * Constructs a rectangle on the given pixel matrix.
	 *
	 * @param matrix  	 The pixel matrix for drawing the rectangle.
	 * @param geometry   The geometry of the rectangle.
	 */
	NRectangle(NGenericPixelMatrix *matrix, const NPixelBox geometry) :
			NShape(matrix), geometry_(geometry) {
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
		bool visible = this->isVisible();
		this->hide();
		geometry_ = geometry;
		if(visible)
			this->show();
	}

	NPixelBox geometry() const {
		return geometry_;
	}

	/**
	 * \brief Get the X-Coordinate of base point (left top)
	 */
	S16 baseX() const {
		return geometry_.base().x();
	}

	/**
	 * \brief Get the Y-Coordinate of base point (left top)
	 */
	S16 baseY() const {
		return geometry_.base().y();
	}

	/**
	 * \brief Set the X-Coordinate of base point (left top)
	 */
	void setBaseX(S16 x) {
		bool visible = this->isVisible();
		this->hide();
		geometry_.setBaseX(x);
		if(visible)
			this->show();
	}

	/**
	 * \brief Set the Y-Coordinate of base point (left top)
	*/
	void setBaseY(S16 y) {
		bool visible = this->isVisible();
		this->hide();
		geometry_.setBaseY(y);
		if(visible)
			this->show();
	}

	/**
	 * \brief Set base point (left top)
	 */
	void setBase(NPoint base) {
		bool visible = this->isVisible();
		this->hide();
		geometry_.setBase(base);
		if(visible)
			this->show();
	}

	U16 width() const {
		return geometry_.width();
	}

	void setWidth(U16 width) {
		bool visible = this->isVisible();
		this->hide();
		geometry_.setWidth(width);
		if(visible)
			this->show();
	}

	U16 height() const {
		return geometry_.height();
	}

	void setHeight(U16 height) {
		bool visible = this->isVisible();
		this->hide();
		geometry_.setHeight(height);
		if(visible)
			this->show();
	}
};


/**
 * \brief Rectangle object for lcd.
 */
class NRectangleFilled : public NShape {
private:
	NPixelBox geometry_;

	void showShapeImpl() const {
		draw(pixelMatrix, geometry_, DrawOpt::draw());
	}

	void hideShapeImpl() const {
		draw(pixelMatrix, geometry_, DrawOpt::clear());
	}

	void invertShapeImpl() const {
		draw(pixelMatrix, geometry_, DrawOpt::invert());
	}

public:

	/**
	 * \brief Draw a filled rectangle on given pixel matrix.
	 *
	 * This function lets you draw a rectangle on the given pixel matrix and geometry.
	 * Optionally specify drawing options.
	 * If this argument is not specified it defaults to DrawOpt::draw().
	 * Valid display options are listed in the DrawOpt class.
	 *
	 * @param matrix  		The pixel matrix for drawing the rectangle.
	 * @param geometry   	The geometry of the rectangle.
	 * @param op   		 	The optional drawing options.
	 */
	static bool draw(NGenericPixelMatrix *matrix, const NPixelBox &geometry, DrawOpt op = DrawOpt::draw());

	/**
	 * \brief Draw a filled rectangle directly on nxt lcd.
	 *
	 * This function lets you draw a rectangle directly on nxt lcd with given geometry.
	 * Optionally specify drawing options.
	 * If this argument is not specified it defaults to DrawOpt::draw().
	 * Valid display options are listed in the DrawOpt class.
	 *
	 * @param geometry   The geometry of the rectangle.
	 * @param op   		 The optional drawing options.
	 */
	static inline bool draw(const NPixelBox &geometry, DrawOpt op = DrawOpt::draw()) {
		NLcd lcd;
		return draw(&lcd, geometry, op);
	}

public:

	/**
	* \brief Construct filled rectangle object on given pixel matrix.
	*
	* Constructs a rectangle on the given pixel matrix.
	*
	* @param matrix     The pixel matrix for drawing the rectangle.
	* @param geometry   The geometry of the rectangle.
	*/
	NRectangleFilled(NGenericPixelMatrix *matrix, const NPixelBox geometry) :
			NShape(matrix), geometry_(geometry) {
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
		bool visible = this->isVisible();
		this->hide();
		geometry_ = geometry;
		if(visible)
			this->show();
	}

	NPixelBox geometry() const {
		return geometry_;
	}

	/**
	 * \brief Get the X-Coordinate of base point (left top)
	 */
	S16 baseX() const {
		return geometry_.base().x();
	}

	/**
	 * \brief Get the Y-Coordinate of base point (left top)
	 */
	S16 baseY() const {
		return geometry_.base().y();
	}

	/**
	 * \brief Set the X-Coordinate of base point (left top)
	 */
	void setBaseX(S16 x) {
		bool visible = this->isVisible();
		this->hide();
		geometry_.setBaseX(x);
		if(visible)
			this->show();
	}

	/**
	 * \brief Set the Y-Coordinate of base point (left top)
	 */
	void setBaseY(S16 y) {
		bool visible = this->isVisible();
		this->hide();
		geometry_.setBaseY(y);
		if(visible)
			this->show();
	}

	/**
	 * \brief Set base point (left top)
	 */
	void setBase(NPoint base) {
		bool visible = this->isVisible();
		this->hide();
		geometry_.setBase(base);
		if(visible)
			this->show();
	}

	U16 width() const {
		return geometry_.width();
	}

	void setWidth(U16 width) {
		bool visible = this->isVisible();
		this->hide();
		geometry_.setWidth(width);
		if(visible)
			this->show();
	}

	U16 height() const {
		return geometry_.height();
	}

	void setHeight(U16 height) {
		bool visible = this->isVisible();
		this->hide();
		geometry_.setHeight(height);
		if(visible)
			this->show();
	}
};


}

#endif /* __NREACTANGLE_HPP_ */
