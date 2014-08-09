/*
 * NPairBox.cpp
 *
 *  Created on: 25.03.2014
 *      Author: Martin
 */

#include "NPairBox.hpp"

namespace nxpl {

void NPairBox::align2Main(NAlignment align) {
	if (align == NAlignment::none()) {
		alignment = align;
		return;
	}

	S8 adjustment = 0;
	if (align == NAlignment::right() || align == NAlignment::left()) {
		if (align == NAlignment::right())
			adjustment = main->textBox().charsInLine(); //!
		else {
			adjustment = -sec->textBox().charsInLine();
		}
		sec->setPosition(NCursor(main->textBox().base().indent() + adjustment, main->textBox().base().line()));
	} else if (align == NAlignment::top() || align == NAlignment::bottom()) {
		adjustment = (align.get() > 0) ? (1) : (-1);
		sec->setPosition(NCursor(main->textBox().base().indent(), main->textBox().base().line() + adjustment));
	}
	alignment = align;
}

}


