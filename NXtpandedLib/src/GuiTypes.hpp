/*
 * GuiTypes.hpp
 *
 *  Created on: 23.11.2013
 *      Author: Martin Aumair
 */


#ifndef __GUITYPES_HPP_
#define __GUITYPES_HPP_

/** \file
 *	\ingroup NxtLcd
*/

namespace nxpl {

/**
 * \brief Draw Options for functions.
 */
class DrawOpt {
private:
	S8 opt;
	explicit DrawOpt(S8 op) : opt(op) {};
public:
	static const S8 drawID = 1;
	static const S8 clearID = 0;
	static const S8 invertID = -1;

	static DrawOpt draw() {return DrawOpt(drawID);}     /**<Normal drawing */
	static DrawOpt clear() {return DrawOpt(clearID);}   /**<Clear pixels while drawing (aka draw in white) */
	static DrawOpt invert() {return DrawOpt(invertID);} /**<Invert pixels*/
	const S8 operator()(void) const {return opt;}       /**<Get ID of the draw option (used for switch/compare)*/
};

}

#endif /* GUITYPES_HPP_ */
