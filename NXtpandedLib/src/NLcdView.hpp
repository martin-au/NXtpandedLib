/*
 * NLcdView.hpp
 *
 *  Created on: 22.02.2014
 *      Author: Martin
 */

#ifndef __NLCDVIEW_HPP_
#define __NLCDVIEW_HPP_

/** \file
 *	\ingroup NxtLcd
*/

#include "NVector.hpp"
#include "NGuiObject.hpp"
#include "Uncopyable.hpp"

namespace nxpl {

/** \brief Group together NWidgets into view.
 *
 * A view is simply a class which manages the show/hide methods of multiple widgets.
 * You can for example implement page switching by changing the actual view.
 */
class NLcdView : public NGuiObject, private Uncopyable {
private:
	NVector<const NGuiObject*> guiObjects;
	static const U8 defaultGuiObjectsBuffer = 6;
	bool emptySpots;

	/** \brief Show all widgets.
	 *
	 * @param update If true update the display.
	 */
	virtual void showImpl() const {
		for(U16 i=0; i < guiObjects.size(); i++) {
			if(guiObjects[i] != static_cast<NGuiObject*>(NULL))
				guiObjects[i]->show();
		}
	}

	/** \brief Hide all widgets.
	 *
	 * @param update If true update the display.
	 */
	virtual void hideImpl() const {
		for(U16 i=0; i < guiObjects.size(); i++) {
			if(guiObjects[i] != static_cast<NGuiObject*>(NULL))
				guiObjects[i]->hide();
		}
	}
public:
	/** Construct a view.
	 *
	 * If you know how many widgets you add to the view pass the number of widgets+1.
	 *
	 * @param widgetsbuffer The starting objects buffer.
	 */
	NLcdView(U8 guiObjectsbuffer = defaultGuiObjectsBuffer) : guiObjects(guiObjectsbuffer), emptySpots(false) {}
	~NLcdView() {}

	/** \brief Add widget to view.
	 *
	 * Remove/add use memory efficient.
	 *
	 * @param myWidget Widget to add.
	 */
	void add(const NGuiObject *myGuiObject) {
		// search for empty spot
		if(emptySpots) {
			S32 idxOfEmpty = guiObjects.indexOf(static_cast<NGuiObject*>(NULL));
			if(idxOfEmpty != guiObjects.size()) {
				guiObjects.set(idxOfEmpty, myGuiObject);
				return;
			} else {
				emptySpots = false;
			}
		}
		guiObjects.pushBack(myGuiObject);
	}

	/** \brief Remove widget to view.
	 *
	 * Remove/add: use memory efficient.
	 *
	 * @param myWidget Widget to add.
	 */
	void remove(const NWidget *myGuiObject) {
		// expensive and deletes the object
		// widgets.removeOne(&myWidget);
		S32 idxOfWidget = guiObjects.indexOf(myGuiObject);
		if(idxOfWidget != guiObjects.size()) {
			guiObjects.set(idxOfWidget, static_cast<NGuiObject*>(NULL));
			emptySpots = true;
		}
	}

	/** \brief Swap views to active/inactive.
	 *
	 * Use this if you have different views and you want to make one view active and one inactive.
	 *
	 * @param actualActive The actual active view (displayed on the screen)
	 * @param actualInactive The actual inactive view which should be now the active view.
	 * @param update If true update the display.
	 */
	static void swap(const NLcdView &actualActive, const NLcdView &actualInactive,
			bool update = false) {
		actualActive.hide();
		actualInactive.show(update);
	}
};


}

#endif /* __NLCDVIEW_HPP_ */
