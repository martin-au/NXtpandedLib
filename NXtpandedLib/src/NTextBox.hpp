/*
 * NTextBox.hpp
 *
 *  Created on: 20.03.2014
 *      Author: Martin
 */

#ifndef __NTEXTBOX_HPP_
#define __NTEXTBOX_HPP_

#include "NCursor.hpp"

namespace nxpl {

/**
 * \brief A field with parameters specified in character width and lines.
 *
 * This class can be used for positioning a widget/text on screen.
 */
class NTextBox {
private:
	NCursor baseLeftTop;
	U8 charsInLineVal, linesVal;

public:

	/**
	 * \brief Construct a NTextBox.
	 *
	 * @param baseLeftTop 	The base point (left top) of the box. Default is zero point.
	 * @param charsInLine 	The width of the box in characters. (max chars in line).
	 * @param height 	    The height of the box in lines.
	 */
	NTextBox(NCursor baseLeftTop = NCursor(0, 0), U8 charsInLine = 1, U8 lines = 1)
	: baseLeftTop(baseLeftTop), charsInLineVal(charsInLine), linesVal(lines) {

	}

	/**
	 * \brief Construct a NTextBox.
	 *
	 * @param indent 		Position of the box based on left top: Indent in characters.
	 * @param line 			Position of the box based on left top: Line.
	 * @param charsInLine 	The width of the box in characters (max chars in line).
	 * @param height 	    The height of the box in lines.
	 */
	NTextBox(S8 indent, S8 line, U8 charsInLine = 1, U8 lines = 1)
	: baseLeftTop(indent, line), charsInLineVal(charsInLine), linesVal(lines) {

	}


	/**
	 * \brief Get text box base position cursor. (left top)
	 */
	NCursor base() const {
		return baseLeftTop;
	}

	/**
	 * \brief Set text box base position cursor. (left top)
	 */
	void setBase(NCursor base) {
		baseLeftTop = base;
	}

	/**
	 * \brief Set text box base position indent. (left top)
	 */
	void setIndent(S8 indent) {
		baseLeftTop.setIndent(indent);
	}

	/**
	 * \brief Set text box base position line. (left top)
	 */
	void setLine(S8 line) {
		baseLeftTop.setLine(line);
	}

	S16 charsInLine() const {
		return charsInLineVal;
	}

	/*
	 * \brief Set text box max chars in line.
	 */
	void setCharsInLine(U8 chars) {
		charsInLineVal = chars;
	}

	S16 lines() const {
		return linesVal;
	}

	/*
	 * \brief Set text box lines.
	 */
	void setLines(U8 lines) {
		linesVal = lines;
	}
};

bool textBoxInLcd(NTextBox box) {
	return LCD::fieldInLcd(box.base().indent(), box.base().line(), box.lines(), box.charsInLine());
}

}

#endif /* __NTEXTBOX_HPP_ */
