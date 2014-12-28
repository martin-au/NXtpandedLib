#ifndef __LCDCONSTANTS_HPP_
#define __LCDCONSTANTS_HPP_

/** \file
 *	\ingroup NxtLcd
*/

// double nesting to avoid namespace pollution
namespace nxpl {
/**
 * \brief Includes all constants and converter functions for the NXT lcd.
 */
namespace LCD {
	const S8 WIDTH = 100;     /**<Lcd width in pixel*/
	const S8 HEIGHT = 64; 	  /**<Lcd height in pixel*/

	const S8 DEPTH = 8; 	  /**<Lcd depth in pixel, depth = line height in pixel*/

	// text constants
	const S8 LINE_WIDTH = 16; 			/**<Line with in chars*/
	const S8 ROWS = HEIGHT / DEPTH;     /**<Rows of NXT lcd*/

	const S8 CHAR_WIDTH = 6;  			/**<Width of one char on the the lcd in pixel*/
	const S8 CHAR_HEIGHT = 8; 			/**<Height of one char on the the lcd in pixel*/

	/**
	 * \brief Convert horizontal pixel coordinate to indent chars.
	 *
	 * Calculates which field the pixel belongs to. First field is 0.
	 * @param x  Pixel X-Coordinate (from left).
	 * @return Indent measured in Chars.
	 */
	inline S8 pixelXToIndent(S8 x) {
		return (x - 1) / LCD::CHAR_WIDTH;
	}

	/**
	 * \brief Convert vertical pixel coordinate to line.
	 *
	 * Calculates which line the pixel belongs to. Top line is 0.
	 * @param y  Pixel Y-Coordinate (from top).
	 * @return Line (0 - LCD::ROWS).
	 */
	inline S8 pixelYToLine(S8 y) {
		return y/LCD::DEPTH;
	}

	/**
	 * \brief Width in pixels to chars
	 *
	 * Calculates the maximum of chars in the given width in pixels.
	 * @param width  Width in pixels.
	 * @return Number of chars.
	 */
	inline S8 pixelWidthToTextWidth(S8 width) {
		return width/LCD::CHAR_WIDTH;
	}

	/**
	 * \brief Calculates the pixel coordinate from line.
	 *
	 * @param line (0 - LCD::ROWS)
	 * @return Pixel Y-Coordinate.
	 */
	inline S8 lineToPixelY(S8 line) {
		return line*LCD::DEPTH;
	}

	/**
	 * \brief Calculates the pixel coordinate from indent.
	 *
	 * @param indent (0 - LCD::LINE_WIDTH)
	 * @return Pixel X-Coordinate.
	 */
	inline S8 indentToPixelX(S8 indent) {
		return indent * LCD::CHAR_WIDTH + 1;
	}

	/** \brief Checks if Pixel-Coordinate is in lcd.
	 *
	 * @param x Pixel X-Coordinate
	 * @param y Pixel Y-Coordinate
	 * @return true if Pixel in lcd.
	 */
	inline bool pixelInLcd(S16 x, S16 y) {
		return ((x >= 0) && (x < LCD::WIDTH) && (y >= 0) && (y < LCD::HEIGHT));
	}

	/**
	 * \brief Checks if cursor position is in lcd.
	 *
	 * @param indent Cursor Indent in chars (0 - LCD::LINE_WIDTH).
	 * @param row (0 - LCD::ROWS)
	 * @return true if cursor is in lcd.
	 */
	inline bool cursorInLcd(S16 indent, S16 row) {
		return ((indent >= 0) && (indent < LCD::LINE_WIDTH) && (row >= 0)
				&& (row < LCD::ROWS));
	}

	/** \brief Checks if field is in lcd.
	 *
	 * @param indent Indent of top left char.
	 * @param row Row of top left char.
	 * @param rows
	 * @param textwidth
	 * @return true if field is in lcd.
	 */
	inline bool fieldInLcd(S16 indent, S16 row, S16 rows, S16 textwidth) {
		return cursorInLcd(indent, row) && cursorInLcd(indent + textwidth, row)
				&& cursorInLcd(indent, row + (rows - 1))
				&& cursorInLcd(indent + textwidth, row + (rows - 1));
	}

	/** \brief Checks if pixel field is in lcd.
	 *
	 * @param x X-Coordinate of top left pixel.
	 * @param y Y-Coordinate of top left pixel.
	 * @param height Height in pixels.
	 * @param width  Width in pixels.
	 * @return true if all pixels are in lcd.
	 */
	inline bool objectInLcd(S16 x, S16 y, S16 height, S16 width) {
		return pixelInLcd(x, y) && pixelInLcd(x + width, y)
				&& pixelInLcd(x, y + height) && pixelInLcd(x + width, y + height);
	}
}
}

#endif
