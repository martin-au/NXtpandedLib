
#ifndef __LCDDRAWER_HPP_
#define __LCDDRAWER_HPP_

#include "NPoint.hpp"
#include "NPixelBox.hpp"

#include "NLcd.hpp"
#include "GuiTypes.hpp"

/** \file
 *	\ingroup NxtLcd
*/

namespace nxpl {

/**
 * \brief Draw a line.
 *
 * This function lets you draw a line on the given lcd from x0, y0 to x1, y1.
 * Optionally specify drawing options.
 * If this argument is not specified it defaults to DrawOpt::draw().
 * Valid display options are listed in the DrawOpt class.
 *
 * @param lcd  The lcd for drawing the line.
 * @param x0   The x value for the start of the line.
 * @param y0   The y value for the start of the line.
 * @param x1   The x value for the end of the line.
 * @param y1   The y value for the end of the line.
 * @param op   The optional drawing options.
 */
bool drawLine(NLcd &lcd, NPoint start, NPoint end, DrawOpt op = DrawOpt::draw());

/**
 * \brief Draw a rectangle.
 *
 * This function lets you draw a rectangle on the given lcd at x, y with the
 * specified width and height.
 * Optionally specify drawing options.
 * If this argument is not specified it defaults to DrawOpt::draw().
 * Valid display options are listed in the DrawOpt class.
 *
 * @param lcd  The lcd for drawing the rectangle.
 * @param x0   The x value for the top left corner of the rectangle.
 * @param y0   The y value for the top left corner of the rectangle.
 * @param w    The width of the rectangle.
 * @param h    The height of the rectangle.
 * @param op   The optional drawing options.
 */
bool drawRectangle(const NLcd &lcd, NPixelBox geometry, DrawOpt op = DrawOpt::draw());

/**
 * \brief Draw a filled rectangle.
 *
 * This function lets you draw a filled rectangle on the given lcd at x, y with the
 * specified width and height.
 * Optionally specify drawing options.
 * If this argument is not specified it defaults to DrawOpt::draw().
 * Valid display options are listed in the DrawOpt class.
 *
 * @param lcd  The lcd for drawing the rectangle.
 * @param x0   The x value for the top left corner of the rectangle.
 * @param y0   The y value for the top left corner of the rectangle.
 * @param w    The width of the rectangle.
 * @param h    The height of the rectangle.
 * @param op   The optional drawing options.
 */
bool drawRectangleFilled(NLcd &lcd, const NPixelBox &geometry, DrawOpt op = DrawOpt::draw());

/** \brief Draw a circle.
 *
 * This function lets you draw a circle on the given lcd with its center at the specified x and y location, using the specified radius.
 * Optionally specify drawing options.
 * If this argument is not specified it defaults to DrawOpt::draw().
 * Valid display options are listed in the DrawOpt class.
 *
 * @param lcd      The lcd for drawing the circle.
 * @param centerX  The x value for the center of the circle.
 * @param centerY  The y value for the center of the circle.
 * @param radius   The radius of the circle.
 * @param op       	The optional drawing options.
 */
bool drawCircle(NLcd &lcd, NPoint center, S8 radius, DrawOpt op = DrawOpt::draw());


/** \brief Draw a filled circle.
 *
 * This function lets you draw a filled circle on the given lcd with its center at the specified x and y location, using the specified radius.
 * Optionally specify drawing options.
 * If this argument is not specified it defaults to DrawOpt::draw().
 * Valid display options are listed in the DrawOpt class.
 *
 * @param lcd      The lcd for drawing the circle.
 * @param centerX  The x value for the center of the circle.
 * @param centerY  The y value for the center of the circle.
 * @param radius   The radius of the circle.
 * @param op       	The optional drawing options.
 */
bool drawCircleFilled(NLcd &lcd, NPoint center, S8 radius, DrawOpt op = DrawOpt::draw());

/** \brief Draw an ellipse.
 *
 * This function lets you draw an ellipse on the screen with its center at the specified x and y location, using the specified radii.
 * Optionally specify drawing options.
 * If this argument is not specified it defaults to DrawOpt::draw().
 * Valid display options are listed in the DrawOpt class.
 *
 * @param lcd      The lcd for drawing the ellipse.
 * @param centerX  The x value for the center of the ellipse.
 * @param centerY  The y value for the center of the ellipse.
 * @param a		   The x axis radius.
 * @param b		   The y axis radius.
 * @param op       The optional drawing options.
 */
bool drawEllipse(NLcd &lcd, NPoint center, S8 a, S8 b, DrawOpt op = DrawOpt::draw());


/** \brief Draw a filled ellipse.
 *
 * This function lets you draw a filled ellipse on the screen with its center at the specified x and y location, using the specified radii.
 * Optionally specify drawing options.
 * If this argument is not specified it defaults to DrawOpt::draw().
 * Valid display options are listed in the DrawOpt class.
 *
 * @param lcd      The lcd for drawing the ellipse.
 * @param centerX  The x value for the center of the ellipse.
 * @param centerY  The y value for the center of the ellipse.
 * @param a		   The x axis radius.
 * @param b		   The y axis radius.
 * @param op       The optional drawing options.
 */
bool drawEllipseFilled(NLcd &lcd, NPoint center, S8 a, S8 b, DrawOpt op = DrawOpt::draw());

} // namespace

#endif
