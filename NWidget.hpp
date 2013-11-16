/*
 * NWidget.hpp
 *
 *  Created on: 16.10.2013
 *      Author: Martin
 */

#ifndef __NWIDGET_HPP_
#define __NWIDGET_HPP_

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

// NWidget is only a virtual object, its designed as a base class which provides
// space managing functions
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

	static const S8 LCD_WIDTH = 100;
	static const S8 LCD_HEIGHT = 64;
	// Max cursor position in X(horizontal) axis.
	static const S8 LCD_TEXT_WIDTH = 16;
	// Max cursor position in Y(vertical) axis.
	static const S8 LCD_ROWS = 8;

	static const S8 CHAR_WIDTH = 6;
	static const S8 LCD_DEPTH = 8;


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
		x = 1 + (rowX * CHAR_WIDTH);
		y = rowY * LCD_DEPTH;
		width = textWidth * CHAR_WIDTH;
		height = rows * LCD_DEPTH;
	}

	void setPixelField(const S8 X, const S8 Y, const S8 Width, const S8 Height) {
		// to pixel
		x = X;
		y = Y;
		width = Width;
		height = Height;
	}

	static inline S8 pixelToLine(S8 y) {
		return y/LCD_DEPTH;
	}

	static bool pixelInLcd(S8 X, S8 Y) {
		return ((X >= 0) && (X < LCD_WIDTH) && (Y >= 0) && (Y < LCD_HEIGHT));
	}
	static bool cursorInLcd(S8 indent, S8 row) {
		return ((indent >= 0) && (indent < LCD_TEXT_WIDTH) && (row >= 0)
				&& (row < LCD_ROWS));
	}
	static bool fieldInLcd(S8 indent, S8 row, S8 rows, S8 charWidth) {
		return cursorInLcd(indent, row) && cursorInLcd(indent + charWidth, row)
				&& cursorInLcd(indent, row + (rows - 1))
				&& cursorInLcd(indent + charWidth, row + (rows - 1));
	}
	static bool objectInLcd(S8 X, S8 Y, S8 Height, S8 Width) {
		return pixelInLcd(X, Y) && pixelInLcd(X + Width, Y)
				&& pixelInLcd(X, Y + Height) && pixelInLcd(X + Width, Y + Width);
	}
	bool inLcd() const {
		return objectInLcd(this->x, this->y, this->height, this->width);
	}
};


#endif /* NWIDGET_HPP_ */
