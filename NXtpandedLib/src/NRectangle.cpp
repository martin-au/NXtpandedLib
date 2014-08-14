/*
 * NRectangle.cpp
 *
 *  Created on: 14.08.2014
 *      Author: Martin
 */

#include "NRectangle.hpp"
#include "NLine.hpp"

namespace nxpl {

bool NRectangle::draw(NLcd &lcd, NPixelBox geometry, DrawOpt op) {
	if(!pixelBoxInLcd(geometry)) return false;

	NPoint rightTop(geometry.base().x() + geometry.width(), geometry.base().y());
	NPoint rightBottom(geometry.base().x() + geometry.width(), geometry.base().y() + geometry.height());
	NPoint leftBottom(geometry.base().x(), geometry.base().y() + geometry.height());

	NLine::draw(lcd, geometry.base(), rightTop, op);
	NLine::draw(lcd, rightTop.setY(rightTop.y() + 1), rightBottom, op);
	NLine::draw(lcd, rightBottom.setX(rightBottom.x() - 1), leftBottom, op);
	NLine::draw(lcd, leftBottom.setY(leftBottom.y()-1), geometry.base().setY(geometry.base().y()+1), op);
	return true;
}

bool NRectangleFilled::draw(NLcd &lcd, const NPixelBox &geometry, DrawOpt op) {
	if(!NRectangle::draw(lcd, geometry, op)) return false;

	S8 xlast = geometry.base().x() + geometry.width();
	S8 ylast = geometry.base().y() + geometry.height();
	for (S8 iy = geometry.base().y() + 1; iy < ylast; ++iy) {
		// straight line
		for (S8 ix = geometry.base().x() + 1; ix < xlast; ++ix) {
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
		}
	}
	return true;
}

}
