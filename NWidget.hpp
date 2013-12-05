/*
 * NWidget.hpp
 *
 *  Created on: 16.10.2013
 *      Author: Martin
 */

#ifndef __NWIDGET_HPP_
#define __NWIDGET_HPP_


// for static const
#include "NLcd.hpp"

/*
 * TODO: Introduce polymorph with virtual ..?
 * TODO Higher encapsulation (no protected data)
 */

class NAlignment {
private:
	S8 val;
public:
	explicit NAlignment(S8 align) : val(align) {};

	static NAlignment none()    { return NAlignment(0); }
	static NAlignment right() 	{ return NAlignment(1); }
	static NAlignment left() 	{ return NAlignment(-1); }
	static NAlignment top() 	{ return NAlignment(-2); }
	static NAlignment bottom() 	{ return NAlignment(2); }

	bool operator==(const NAlignment& rhs) const {
		return (val == rhs.val);
	}

	inline S8 get() const { return val; }
};

// NWidget is designed as a base class which provides space managing functions
// TODO: make all protected private
class NWidget {

private:
	// in pixel
	S8 mX, mY;
	S8 mHeight, mWidth;

	S8 rowX, rowY;
	S8 rows, textWidth;

	mutable bool mVisible; // change allowed during show() const!
protected:

	void setVisibility(bool visible) const {
		mVisible = visible;
	}
	// virtual functions would be better but to much overhead in embedded c++!
	void x(S8 setX) {
		mX = setX;
	}

	void y(S8 setY) {
		mY = setY;
	}

	void height(S8 setHeight) {
		mHeight = setHeight;
	}

	void width(S8 setWidth) {
		mWidth = setWidth;
	}

	void indent(S8 setIndent) {
		mX = 1 + (setIndent * LCD::CHAR_WIDTH);
		rowX = setIndent;
	}

	void row(S8 setRow) {
		mY = setRow * LCD::DEPTH;
		rowY = setRow;
	}

	void lines(S8 setLines) {
		mHeight = setLines * LCD::DEPTH;
		rows = setLines;
	}

	void fieldWidth(S8 setFieldWidth) {
		mWidth = setFieldWidth * LCD::CHAR_WIDTH;
		textWidth = setFieldWidth;
	}

public:
	static const S8 keep = -1;

	NWidget() : mX(0), mY(0), mHeight(0), mWidth(0), rowX(0), rowY(), rows(0), textWidth(0), mVisible(0) {}

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

	bool isVisible() const {
		return mVisible;
	}

	void setField(S8 indentX, S8 rowY, S8 nrows, S8 nchars) {
		indent(indentX);
		row(rowY);
		lines(nrows);
		fieldWidth(nchars);

	}

	void setPixelField(S8 x, S8 y, S8 width, S8 height) {
		this->x(x);
		this->y(y);
		this->width(width);
		this->height(height);
	}
	bool inLcd() const {
		return LCD::objectInLcd(x(), y(), height(), width());
	}
};


#endif /* NWIDGET_HPP_ */
