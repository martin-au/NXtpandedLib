/*
 * GuiTypes.hpp
 *
 *  Created on: 23.11.2013
 *      Author: Martin
 */

#ifndef __GUITYPES_HPP_
#define __GUITYPES_HPP_

namespace nxpl {

/**
 * \brief Draw Options for functions.
 */
class DrawOpt {
	S8 opt;
private:
	explicit DrawOpt(S8 op) : opt(op) {};
public:
	static const S8 drawID = 1;
	static const S8 clearID = 0;
	static const S8 invertID = -1;

	static DrawOpt draw() {return DrawOpt(drawID);}
	static DrawOpt clear() {return DrawOpt(clearID);}
	static DrawOpt invert() {return DrawOpt(invertID);}
	const S8 operator()(void) const {return opt;}
};

}

#endif /* GUITYPES_HPP_ */
