
#ifndef __LCDCONSTANTS_HPP_
#define __LCDCONSTANTS_HPP_

namespace LCD {
	const S8 WIDTH = 100;
	const S8 HEIGHT = 64;

	const S8 DEPTH = 8;

	// text constants
	const S8 LINE_WIDTH = 16;
	const S8 ROWS = DEPTH;

	// one char
	const S8 CHAR_WIDTH = 6;
	const S8 CHAR_HEIGHT = 8;

	inline S8 pixelToLine(const S8 y) {
		return y/LCD::DEPTH;
	}

	inline S8 lineToPixel(const S8 line) {
		return line*LCD::DEPTH;
	}

	bool pixelInLcd(const S16 x, const S16 y) {
		return ((x >= 0) && (x < LCD::WIDTH) && (y >= 0) && (y < LCD::HEIGHT));
	}

	bool cursorInLcd(const S16 indent, const S16 row) {
		return ((indent >= 0) && (indent < LCD::LINE_WIDTH) && (row >= 0)
				&& (row < LCD::ROWS));
	}
	bool fieldInLcd(const S16 indent, const S16 row, const S16 rows, const S16 textwidth) {
		return cursorInLcd(indent, row) && cursorInLcd(indent + textwidth, row)
				&& cursorInLcd(indent, row + (rows - 1))
				&& cursorInLcd(indent + textwidth, row + (rows - 1));
	}
	bool objectInLcd(const S16 x, const S16 y, const S16 height, const S16 width) {
		return pixelInLcd(x, y) && pixelInLcd(x + width, y)
				&& pixelInLcd(x, y + height) && pixelInLcd(x + width, y + height);
	}
}

#endif
