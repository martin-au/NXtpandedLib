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

class NTextBox {
private:
	NCursor baseLeftTop;
	U8 charsInLineVal, linesVal;

public:
	NTextBox(NCursor baseLeftTop = NCursor(0, 0), U8 charsInLine = 1, U8 lines = 1)
	: baseLeftTop(baseLeftTop), charsInLineVal(charsInLine), linesVal(lines) {

	}

	NTextBox(S8 indent, S8 line, U8 charsInLine = 1, U8 lines = 1)
	: baseLeftTop(indent, line), charsInLineVal(charsInLine), linesVal(lines) {

	}

	NCursor base() const {
		return baseLeftTop;
	}

//	NCursor & base() {
//		return baseLeftTop;
//	}

	void setBase(NCursor base) {
		baseLeftTop = base;
	}

	void setIndent(S8 indent) {
		baseLeftTop.setIndent(indent);
	}

	void setLine(S8 line) {
		baseLeftTop.setLine(line);
	}

	S16 charsInLine() const {
		return charsInLineVal;
	}

	void setCharsInLine(U8 chars) {
		charsInLineVal = chars;
	}

	S16 lines() const {
		return linesVal;
	}

	void setLines(U8 lines) {
		linesVal = lines;
	}
};

bool textBoxInLcd(NTextBox box) {
	return LCD::fieldInLcd(box.base().indent(), box.base().line(), box.lines(), box.charsInLine());
}

}

#endif /* __NTEXTBOX_HPP_ */
