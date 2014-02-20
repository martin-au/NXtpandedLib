
#ifndef __LCDDRAWER_HPP_
#define __LCDDRAWER_HPP_

#include "NLcd.hpp"
#include "GuiTypes.hpp"
#include "C:/cygwin/GNUARM/include/c++/4.0.2/cstdlib" // abs?

namespace nxpl {

void drawLine(NLcd lcd, S8 x0, S8 y0, S8 x1, S8 y1, DrawOpt op);

void drawRectangle(NLcd lcd, S8 x0, S8 y0, S8 w, S8 h, DrawOpt op);

void drawRectangleFilled(NLcd lcd, S8 x0, S8 y0, S8 w, S8 h, DrawOpt op);

void drawCircle(NLcd lcd, S8 centerX, S8 centerY, S8 radius, DrawOpt op);

// TODO work here with functors?
void drawCircleFilled(NLcd lcd, S8 centerX, S8 centerY, S8 radius, DrawOpt op);

void drawEllipse(NLcd lcd, S8 centerX, S8 centerY, S8 a, S8 b, DrawOpt op);

// TODO work with functors
void drawEllipseFilled(NLcd lcd, S8 centerX, S8 centerY, S8 a, S8 b, DrawOpt op);

} // namespace

#endif
