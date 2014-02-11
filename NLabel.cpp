/*
 * NLabel.cpp
 *
 *  Created on: 16.10.2013
 *      Author: Martin
 */

#include "NLabel.hpp"

NLabel::NLabel(const S8 indent, const S8 row, S8 charWidth) :
		label(new NString(charWidth)), buddy(0), buddyAlignment(0) {
	setField(indent, row, 1, charWidth);
}


NLabel::NLabel(const NString &text, const S8 indent, const S8 row, S8 charWidth) :
		label(new NString(text)) , buddy(0), buddyAlignment(0) {
	if (!charWidth) {
		charWidth = static_cast<S8>(label->size());
	}
	setField(indent, row, 1, charWidth);
}

NLabel::NLabel(const char *text, const S8 indent, const S8 row, S8 charWidth) :
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
	setField(indent, row, 1, charWidth);
}


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

NLabel::~NLabel() {
	// remove from display
	label->clear();
	show();
	delete label;
}

NString NLabel::getText() const {
	return *label;
}


void NLabel::setText(const NString &text) {
	if (text.size() > 0) {
		*label = text;
	}
}

void NLabel::setText(const char *text) {
	if (text) {
		*label = text;
	}
}

void NLabel::clear() {
	label->clear();    // !const
	erase(false);
}

// its better to know the position at construction time!
bool NLabel::setPosition(const S8 indent, const S8 row) {
	erase();

	setField(indent, row, 1, this->fieldWidth());
	if (buddy)
		buddy->alignBuddy(buddyAlignment);
	return inLcd();
}


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


void NLabel::show(bool update) const {
	const S8 labelLen = static_cast<S8>(label->size());

	// we have to change label here because the width of the label
	// can change between setText and show

	bool endString = false;
	for(S16 i=0; i < this->fieldWidth(); ++i) {
		display_goto_xy(this->indent()+i, this->row());
		if(!endString && label[i] == '\0') {
			endString = true;
			continue;
		}
		if(endString) {
			// clean rest of field
			display_char(' ');
		} else {
			display_char(label[i]);
		}
	}

	/*
	if (this->fieldWidth() == labelLen) {
		display_string(label->data());
	} else if (this->fieldWidth() < labelLen) {
		display_string(label->substr(0, this->fieldWidth()).data());
	} else if (this->fieldWidth() > labelLen) {
		NString dispStr(this->fieldWidth());
		dispStr = *label;
		for (S8 i = 0; i < (this->fieldWidth() - labelLen); ++i) {
			dispStr.append(' ');
		}

		display_string(dispStr.data());
	}
	*/
	if (update) {
		display_update();
	}
	setVisibility(true);
}


// TODO use display_char here (faster, lighter)
void NLabel::erase(bool update) const {
	// clean up (but save string)
	char clear[this->fieldWidth() + 1];
	for (S8 i = 0; i < this->fieldWidth(); ++i)
		clear[i] = ' ';
	clear[this->fieldWidth()] = '\0';
	display_goto_xy(this->indent(), this->row());
	display_string(clear);
	if (update) {
		display_update();
	}
	setVisibility(false);
}





