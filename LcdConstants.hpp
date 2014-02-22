
#ifndef __LCDCONSTANTS_HPP_
#define __LCDCONSTANTS_HPP_

// double nesting to avoid namespace pollution
namespace nxpl {
namespace LCD {
	const S8 WIDTH = 100;     /**<LCD width in pixel*/
	const S8 HEIGHT = 64; 	  /**<LCD height in pixel*/

	const S8 DEPTH = 8; 	  /**<LCD depth in pixel, depth = line height in pixel*/

	// text constants
	const S8 LINE_WIDTH = 16; /**<Line with in chars*/
	const S8 ROWS = DEPTH;    /**<Rows of NXT LCD*/

	const S8 CHAR_WIDTH = 6;  /**<Width of one char on the the LCD in pixel*/
	const S8 CHAR_HEIGHT = 8; /**<Height of one char on the the LCD in pixel*/

	/**
	 * \brief Convert horizontal pixel coordinate to indent chars.
	 *
	 * Calculates which field the pixel belongs to. First field is 0.
	 * @param x  Pixel X-Coordinate (from left).
	 * @return Indent in measured in Chars.
	 */
	S8 pixelToIndent(S8 x) {
		return (x - 1) / LCD::CHAR_WIDTH;
	}

	/**
	 * \brief Convert vertical pixel coordinate to line.
	 *
	 * Calculates which line the pixel belongs to. Top line is 0.
	 * @param y  Pixel Y-Coordinate (from top).
	 * @return Line (0-LCD::DEPTH).
	 */
	S8 pixelToLine(S8 y) {
		return y/LCD::DEPTH;
	}

	/**
	 * \brief Width in pixels to chars
	 *
	 * Calculates the maximum of chars in the given width in pixels.
	 * @param width  Width in pixels.
	 * @return Number of chars.
	 */
	S8 pixelToTextWidth(S8 width) {
		return width/LCD::CHAR_WIDTH;
	}

	/**
	 * \brief Calculates the pixel coordinate from line.
	 *
	 * @param Line (0 - LCD::DEPTH)
	 * @return Pixel Y-Coordinate.
	 */
	S8 lineToPixel(S8 line) {
		return line*LCD::DEPTH;
	}

    // <= height ok??
	/** \brief Checks if Pixel-Coordinate is in Lcd.
	 *
	 * @param x Pixel X-Coordinate
	 * @param y Pixel Y-Coordinate
	 * @return true if Pixel in LCD.
	 */
	bool pixelInLcd(S16 x, S16 y) {
		return ((x >= 0) && (x < LCD::WIDTH) && (y >= 0) && (y <= LCD::HEIGHT));
	}

	/**
	 * \ brief Checks if cursor position is in LCD.
	 * @param indent Cursor Indent in chars.
	 * @param row (0 - LCD::DEPTH)
	 * @return true if cursor is in LCD.
	 */
	bool cursorInLcd(const S16 indent, const S16 row) {
		return ((indent >= 0) && (indent < LCD::LINE_WIDTH) && (row >= 0)
				&& (row < LCD::ROWS));
	}

	/** \brief Checks if field is in LCD.
	 *
	 * @param indent Indent of top left char.
	 * @param row Row of top left char.
	 * @param rows
	 * @param textwidth
	 * @return true if field is in LCD.
	 */
	bool fieldInLcd(S16 indent, S16 row, S16 rows, S16 textwidth) {
		return cursorInLcd(indent, row) && cursorInLcd(indent + textwidth, row)
				&& cursorInLcd(indent, row + (rows - 1))
				&& cursorInLcd(indent + textwidth, row + (rows - 1));
	}

	/** \brief Checks if pixel field is in LCD.
	 *
	 * @param x X-Coordinate of top left pixel.
	 * @param y Y-Coordinate of top left pixel.
	 * @param height Height in Pixels.
	 * @param width  Width in Pixels.
	 * @return true if all pixels are in LCD.
	 */
	bool objectInLcd(const S16 x, const S16 y, const S16 height, const S16 width) {
		return pixelInLcd(x, y) && pixelInLcd(x + width, y)
				&& pixelInLcd(x, y + height) && pixelInLcd(x + width, y + height);
	}
}
}

#endif
