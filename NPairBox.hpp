/*
 * PairBox.hpp
 *
 *  Created on: 12.02.2014
 *      Author: Martin
 */

#ifndef __NPAIRBOX_HPP_
#define __NPAIRBOX_HPP_

#include <C:/cygwin/GNUARM/include/c++/4.0.2/tr1/utility>

#include "NWidget.hpp"

template<class T1, class T2>
class NPairBox {
	//std::pair<T1*, T2*> widget;
	T1* main;
	T2* sec;
public:
	NPairBox(T1 *mainWidget, T2 *secWidget)
		: main(mainWidget), sec(secWidget) {
	}

	bool setPosition(S8 indent, S8 row, const NAlignment align) {
		if(!main->setPosition(indent, row)) {
			return false;
		}
		return align2Main(align);
	}

	bool align2Main(const NAlignment align);

	~NPairBox() {}
};

#endif /* __NPAIRBOX_HPP_ */
