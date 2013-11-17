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

	inline static NAlignment none()    	{ return NAlignment(0); }
	inline static NAlignment right() 	{ return NAlignment(1); }
	inline static NAlignment left() 	{ return NAlignment(-1); }
	inline static NAlignment top() 		{ return NAlignment(-2); }
	inline static NAlignment bottom() 	{ return NAlignment(2); }

	bool operator==(const NAlignment& rhs) const {
		return (val == rhs.val);
	}

	inline S8 get() const { return val; }
};

// NWidget is designed as a base class which provides space managing functions
// TODO: make all protected private
class NWidget {
protected:
	// in Pixels
	S8 x, y;
	S8 height, width;
	// in Lines
	S8 rowX, rowY;
	S8 rows, textWidth;

	mutable bool visible; // change allowed during show() const!

public:


	S8 getX() const {
		return x;
	}
	S8 getY() const {
		return y;
	}
	S8 getHeight() const {
		return height;
	}
	S8 getWidth() const {
		return width;
	}
	S8 getRowIndent() const {
		return rowX;
	}
	S8 getRow() const {
		return rowY;
	}
	S8 getRows() const {
		return rows;
	}
	S8 getFieldWidth() const {
		return textWidth;
	}
	bool isVisible() const {
		return visible;
	}

	void setField(const S8 indent, const S8 row, const S8 nrows, const S8 charWidth) {
		rowX = indent;
		rowY = row;
		rows = nrows;
		textWidth = charWidth;
		// to pixel
		x = 1 + (rowX * LCD::CHAR_WIDTH);
		y = rowY * LCD::DEPTH;
		width = textWidth * LCD::CHAR_WIDTH;
		height = rows * LCD::DEPTH;
	}

	void setPixelField(const S8 X, const S8 Y, const S8 Width, const S8 Height) {
		// to pixel
		x = X;
		y = Y;
		width = Width;
		height = Height;
	}
	bool inLcd() const {
		return LCD::objectInLcd(this->x, this->y, this->height, this->width);
	}
};


#endif /* NWIDGET_HPP_ */
