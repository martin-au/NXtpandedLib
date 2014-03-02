/*
 * NWidget.hpp
 *
 *  Created on: 16.10.2013
 *      Author: Martin
 */

#ifndef __NWIDGET_HPP_
#define __NWIDGET_HPP_

/** \file
 *	\ingroup NxtLcd
*/

// for static const
#include "NLcd.hpp"

extern "C" int __cxa_pure_virtual(){return 0;}

namespace nxpl {

// NWidget is designed as a base class which provides space managing functions
// Be aware: All coordinates are converted into pixel coordinates (100x64)
// but never in other direction! If you set for example x, height then the functions indent, lines will return 0!
// You can convert Text-Coordinates into pixel coordinates with data loss: see LcdConstants.hpp
/** \brief Base class for lcd-widgets.
 *
 * The class holds the position of the widget and allows to manage the space of the lcd.
 * Every derived class must set the field with NWidget::setField.
 * Be aware: All coordinates are converted into pixel coordinates (100x64) but never in other direction!
 * If you set for example height in pixels then the function lines will return 0
 * You can convert Text-Coordinates into pixel coordinates with data loss: see LcdConstants.hpp
 * <br>
 * const NWidget means constant position but not constant visibility.
 * <br>
 * It also specifies the two pure virtual functions NWidget::show() and NWidget::hide() which every derived class must implement.
 */
class NWidget {

private:
	// in pixel
	S8 mX, mY;
	S8 mHeight, mWidth;

	S8 rowX, rowY;
	S8 rows, textWidth;

	mutable bool mVisible; // change allowed during show() const!
protected:
	/*
	NWidget(S8 setX, S8 setY, S8 setHeight, S8 setWidth)
		: mX(setX),
		  mY(setY),
		  mHeight(setHeight),
		  mWidth(setWidth),
		  rowX(0),
		  rowY(),
		  rows(0),
		  textWidth(0),
		  mVisible(0) {}
	NWidget(S8 setIndent, S8 setRow, S8 setLines, S8 setFieldWidth) {
		setField(setIndent, setRow, setLines, setFieldWidth);
	}
	*/

	/** \brief Set widget to visible/invisible.
	 *
	 * @param visible true if visible.
	 */
	void setVisibility(bool visible) const {
		mVisible = visible;
	}

	/** \brief Set x-Coordinate.
	 *
	 * @param setX X-Coordinate from left side in pixels.
	 */
	void x(S8 setX) {
		mX = setX;
	}

	/** \brief Set y-Coordinate.
	 *
	 * @param setY Y-Coordinate from top in pixels.
	 */
	void y(S8 setY) {
		mY = setY;
	}

	/** \brief Set height.
	*
	* @param setHeight Height in pixels.
	*/
	void height(S8 setHeight) {
		mHeight = setHeight;
	}

	/** \brief Set width.
	*
	* @param setWidth Width in pixels.
	*/
	void width(S8 setWidth) {
		mWidth = setWidth;
	}

	/** \brief Set indent.
	*
	* @param setIndent Indent in chars.
	*/
	void indent(S8 setIndent) {
		mX = 1 + (setIndent * LCD::CHAR_WIDTH);
		rowX = setIndent;
	}

	/** \brief Set row.
	*
	* @param setRow Row in rows.
	*/
	void row(S8 setRow) {
		mY = setRow * LCD::DEPTH;
		rowY = setRow;
	}

	/** \brief Set lines.
	*
	* @param setLines Number of lines.
	*/
	void lines(S8 setLines) {
		mHeight = setLines * LCD::DEPTH;
		rows = setLines;
	}

	/** \brief Set field width.
	*
	* @param setFieldWidth Field width in chars.
	*/
	void fieldWidth(S8 setFieldWidth) {
		mWidth = setFieldWidth * LCD::CHAR_WIDTH;
		textWidth = setFieldWidth;
	}

	/** \brief Set (text) field.
	 *
	 * @param indentX Indent in chars.
	 * @param rowY Row in rows.
	 * @param nrows Number of lines.
	 * @param nchars  Field width in chars.
	 */
	void setField(S8 indentX, S8 rowY, S8 nrows, S8 nchars) {
		indent(indentX);
		row(rowY);
		lines(nrows);
		fieldWidth(nchars);
	}

	/** \brief Set pixel field.
	 *
	 * @param x X-Coordinate from left side in pixels.
	 * @param y Y-Coordinate from top in pixels.
	 * @param width Width in pixels.
	 * @param height Height in pixels.
	 */
	void setPixelField(S8 x, S8 y, S8 width, S8 height) {
		this->x(x);
		this->y(y);
		this->width(width);
		this->height(height);
	}

public:
	static const S8 keep = -1; //*<*/

	NWidget() : mX(0), mY(0), mHeight(0), mWidth(0), rowX(0), rowY(), rows(0), textWidth(0), mVisible(0) {}
	virtual ~NWidget() {}

	/** \brief Make the widget visible.
	 *
	 * @param update If true update the display.
	 */
	virtual void show(bool update = false) const = 0;

	/** \brief Make the widget invisible.
	 *
	 * @param update If true update the display.
	 */
	virtual void hide(bool update = false) const = 0;

	S8 x() const {
		return mX;
	}

	S8 y() const {
		return mY;
	}

	S8 height() const {
		return mHeight;
	}

	S8 width() const {
		return mWidth;
	}


	/// test functions to use functions and not variables
	S8 indent() const {
		//return (getX() - 1) / LCD::CHAR_WIDTH;
		return rowX;
	}

	S8 row() const {
		//return LCD::pixelToLine(getY());
		return rowY;
	}

	S8 lines() const {
		//return LCD::pixelToLine(getHeight());
		return rows;
	}

	S8 fieldWidth() const {
		//return getWidth()/LCD::CHAR_WIDTH;
		return textWidth;
	}

	/** \brief Checks if widget is visible.
	 *
	 * @return true if visible.
	 */
	bool isVisible() const {
		return mVisible;
	}

	/** \brief Checks if field is in lcd.
	 *
	 * @return true if in lcd.
	 */
	bool inLcd() const {
		return LCD::objectInLcd(x(), y(), height(), width());
	}
};

}

#endif /* NWIDGET_HPP_ */
