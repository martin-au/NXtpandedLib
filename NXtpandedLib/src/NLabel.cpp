/*
 * NLabel_new.cpp
 *
 *  Created on: 21.03.2014
 *      Author: Martin
 */


#include "NLabel.hpp"

namespace nxpl {

void NLabel::showWidgetImpl() const {
	if(!somenew) return;
	for(S16 i=0; i < textBox().charsInLine(); ++i) {
		display_goto_xy(textBox().base().indent() + i, textBox().base().line());
		if(i >= label->size()) {
			display_char(' ');
		}
		else {
			display_char(label->at(i));
		}
	}
}

void NLabel::hideWidgetImpl() const {
	for(S16 i=0; i < textBox().charsInLine(); ++i) {
		display_goto_xy(textBox().base().indent()+i, textBox().base().line());
		display_char(' ');
	}
}

}



