/*
 * NTextBox.hpp
 *
 *  Created on: 20.03.2014
 *      Author: Martin
 */

#ifndef __NTEXTBOX_HPP_
#define __NTEXTBOX_HPP_

/*
 * NPixelBox.hpp
 *
 *  Created on: 19.03.2014
 *      Author: Martin
 */

#ifndef __NPIXELBOX_HPP_
#define __NPIXELBOX_HPP_

#include "NCursor.hpp"

namespace nxpl {

class NTextBox {
private:
	NCursor baseLeftTop;
	U8 charsInLineVal, linesVal;

public:
	NTextBox(NCursor baseLeftTop, U8 charsInLine, U8 lines)
	: baseLeftTop(baseLeftTop), charsInLineVal(charsInLine), linesVal(lines) {

	}

	NCursor base() const {
		return baseLeftTop;
	}

	NCursor & base() {
		return baseLeftTop;
	}

	void setBase(NCursor base) {
		baseLeftTop = base;
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



}


#endif /* NPIXELBOX_HPP_ */



#endif /* NTEXTBOX_HPP_ */
