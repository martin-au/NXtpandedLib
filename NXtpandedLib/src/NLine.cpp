/*
 * NLine.cpp
 *
 *  Created on: 14.08.2014
 *      Author: Martin
 */

#include "NLine.hpp"
#include "..\..\..\GNUARM\arm-elf\include\stdlib.h" //abs

namespace nxpl {

bool NLine::draw(NLcd &lcd, NPoint start, NPoint end, DrawOpt op) {
	if(!lcd.noError()) return false;
	if(!pointInLcd(start) || !pointInLcd(end)) return false;

	const S8 height = abs(end.y() - start.y());
	const S8 width = abs(end.x() - start.x());

	S8 ix = start.x();
	S8 iy = start.y();
	const S8 sx = ix < end.x() ? 1 : -1;
	const S8 sy = iy < end.y() ? 1 : -1;
	S16 err = width + (-height), e2;

	for (;;) {
		switch (op()) {
		case DrawOpt::drawID:
			lcd.pixelOn(static_cast<U8>(ix), static_cast<U8>(iy));
			break;
		case DrawOpt::clearID:
			lcd.pixelOff(static_cast<U8>(ix), static_cast<U8>(iy));
			break;
		case DrawOpt::invertID:
			lcd.invertPixel(static_cast<U8>(ix), static_cast<U8>(iy));
			break;
		default:
			lcd.pixelOn(static_cast<U8>(ix), static_cast<U8>(iy));
		}

		//lcd.pixelOn(static_cast<U8>(ix), static_cast<U8>(iy));
		if (ix == end.x() && iy == end.y())
			break;
		e2 = 2 * err;
		if (e2 > (-height)) {
			err += (-height);
			ix += sx;
		}
		if (e2 < width) {
			err += width;
			iy += sy;
		}
	}
	return true;
}

}

