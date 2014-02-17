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
struct NPairBox {
	T1* main;
	T2* sec;

	NPairBox(T1 *mainWidget, T2 *secWidget) :
			main(mainWidget), sec(secWidget) {
	}

	NPairBox(T1 *mainWidget, T2 *secWidget, S8 indent, S8 row, const NAlignment align) :
			main(mainWidget), sec(secWidget) {
		setPosition(indent, row, align);
	}

	NPairBox(T1 *mainWidget, T2 *secWidget, const NAlignment align) :
			main(mainWidget), sec(secWidget) {
		align2Main(align);
	}

	~NPairBox() {}

	void show(bool update = false) const {
		main->show(false);
		sec->show(update);
	}

	void hide(bool update = false) const {
		main->hide(false);
		sec->hide(update);
	}

	bool setPosition(S8 indent, S8 row, const NAlignment align) {
		if(!main->setPosition(indent, row)) {
			return false;
		}
		return align2Main(align);
	}

	bool align2Main(const NAlignment align);
};

#include "NPairBox.cpp"

#endif /* __NPAIRBOX_HPP_ */
