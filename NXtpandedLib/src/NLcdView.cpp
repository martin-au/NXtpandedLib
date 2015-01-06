/*
 * NLcdView.cpp
 *
 *  Created on: 06.01.2015
 *      Author: Martin
 */

#include "NLcdView.hpp"

void NLcdView::showImpl() const
{
	for(U16 i=0; i < guiObjectsBufferSize; i++) {
		if(guiObjects[i] != static_cast<NGuiObject*>(NULL))
			guiObjects[i]->show();
	}
}

void NLcdView::hideImpl() const
{
	for(U16 i=0; i < guiObjectsBufferSize; i++) {
		if(guiObjects[i] != static_cast<NGuiObject*>(NULL))
			guiObjects[i]->hide();
	}
}

NLcdView::NLcdView(U8 guiObjectsbuffer) : guiObjectsBufferSize(guiObjectsbuffer)
{
	guiObjects = new const NGuiObject*[guiObjectsBufferSize];

	for(U16 i=0; i < guiObjectsBufferSize; i++)
		guiObjects[i] = static_cast<NGuiObject*>(NULL);
}

void NLcdView::add(const NGuiObject *myGuiObject)
{
	for(U16 i=0; i < guiObjectsBufferSize; i++) {
		if(guiObjects[i] == static_cast<NGuiObject*>(NULL)) {
			guiObjects[i] = myGuiObject;
			break;
		}
	}
}

void NLcdView::remove(const NGuiObject *myGuiObject)
{
	for(U16 i=0; i < guiObjectsBufferSize; i++) {
		if(guiObjects[i] == myGuiObject) {
			guiObjects[i] = static_cast<NGuiObject*>(NULL);
			break;
		}
	}
}
