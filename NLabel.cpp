/*
 * NLabel.cpp
 *
 *  Created on: 16.10.2013
 *      Author: Martin
 */

#include "NLabel.hpp"

namespace nxpl {

NLabel::NLabel(S8 indent, S8 row, S8 charWidth) :
		label(new NString(charWidth)), somenew(true) { // , buddy(0), buddyAlignment(0)
 	setField(indent, row, 1, charWidth);
}


NLabel::NLabel(const NString &text, S8 indent, S8 row, S8 charWidth) :
		label(new NString(text)), somenew(true) {
	if (!charWidth) {
		charWidth = static_cast<S8>(label->size());
	}
	setField(indent, row, 1, charWidth);
}

NLabel::NLabel(const char *text, S8 indent, S8 row, S8 charWidth) :
	somenew(true) {
	if (text) {
		label = new NString(text);
	} else {
		if (charWidth)
			label = new NString(charWidth);
		else
			label = new NString(5);
	}

	if (!charWidth) {
		charWidth = static_cast<S8>(label->size());
	}
	setField(indent, row, 1, charWidth);
}


/*
NLabel::NLabel(NLabel * const buddyOf, S8 charWidth) :
		label(new NString(charWidth)), buddy(0), buddyAlignment(0) {
	this->fieldWidth(charWidth);
	if (buddyOf) {
		buddyOf->setBuddy(this);
	}
}


NLabel::NLabel(const NString &text, NLabel * const buddyOf, S8 charWidth) :
		label(new NString(text)), buddy(0), buddyAlignment(0) {
	if (!charWidth) {
		charWidth = static_cast<S8>(label->size());
	}
	this->fieldWidth(charWidth);
	if (buddyOf) {
		buddyOf->setBuddy(this);
	}
}

NLabel::NLabel(const char *text, NLabel * const buddyOf, S8 charWidth) :
		buddy(0), buddyAlignment(0) {
	if (text) {
		label = new NString(text);
	} else {
		if (charWidth)
			label = new NString(charWidth);
		else
			label = new NString(5);
	}

	if (!charWidth) {
		charWidth = static_cast<S8>(label->size());
	}
	this->fieldWidth(charWidth);
	if (buddyOf) {
		buddyOf->setBuddy(this);
	}
}
*/

NLabel::~NLabel() {
	// remove from display
	label->clear();
	show();
	delete[] label;
}


/*
void NLabel::setBuddy(NLabel * const myBuddy, const NAlignment &align) const {
	buddy = myBuddy;
	alignBuddy(align);
}

void NLabel::alignBuddy(const NAlignment &align) const {
	buddyAlignment = align;
	if(align == NAlignment::none()) return;
	S8 adjustment = 0;
	if (align == NAlignment::right() || align == NAlignment::left()) {
		if (align == NAlignment::right())
			adjustment = align.get() + this->fieldWidth() - 1; //!
		else {
			adjustment = align.get() - buddy->fieldWidth() - 1;
		}
		buddy->setPosition(this->indent() + adjustment, this->row());
	} else if (align == NAlignment::top() || align == NAlignment::bottom()) {
		adjustment = (align.get() > 0) ? (1) : (-1);
		buddy->setPosition(this->indent(), this->row() + adjustment);
	}
}
*/

void NLabel::show(bool update) const {
	if(!somenew) return;
	for(S16 i=0; i < this->fieldWidth(); ++i) {
		display_goto_xy(this->indent()+i, this->row());
		if(i >= label->size()) {
			display_char(' ');
		}
		else {
			display_char(label->at(i));
		}
	}

	if (update) {
		display_update();
	}
	setVisibility(true);
}



void NLabel::hide(bool update) const {
	if(!somenew) return;
	for(S16 i=0; i < this->fieldWidth(); ++i) {
		display_goto_xy(this->indent()+i, this->row());
		display_char(' ');
	}
	if (update) {
		display_update();
	}
	setVisibility(false);
}

}



