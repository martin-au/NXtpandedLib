/*
 * NAlignment.hpp
 *
 *  Created on: 02.03.2014
 *      Author: Martin
 */

#ifndef __NALIGNMENT_HPP_
#define __NALIGNMENT_HPP_

/** \file
 *	\ingroup NxtLcd
*/
#include "../../lejos_nxj/src/nxtvm/platform/nxt/mytypes.h"

/**
 * \brief Alignment options for functions.
 */
class NAlignment {
private:
	S8 val;
	explicit NAlignment(S8 align) : val(align) {};
public:
	static NAlignment none()    { return NAlignment(0); }  /**No alignment<*/
	static NAlignment right() 	{ return NAlignment(1); }  /**Align right to object<*/
	static NAlignment left() 	{ return NAlignment(-1); } /**Align left to object<*/
	static NAlignment top() 	{ return NAlignment(-2); } /**Align top to object<*/
	static NAlignment bottom() 	{ return NAlignment(2); }  /**Align bottom to object<*/

	bool operator==(const NAlignment& rhs) const {
		return (val == rhs.val);
	}

	/** \brief Get the id of Alignment.
	 * Used for lib functions internally.
	 * @return id
	 */
	inline S8 getID() const { return val; }
};


#endif /* NALIGNMENT_HPP_ */
