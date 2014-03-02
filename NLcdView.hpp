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
#include "NWidget.hpp"
#include "Uncopyable.hpp"

namespace nxpl {

/** \brief Group together NWidgets into view.
 *
 * A view is simply a class which manages the show/hide methods of multiple widgets.
 * You can for example implement page switching by changing the actual view.
 */
class NLcdView : private Uncopyable {
private:
	NVector<const NWidget*> widgets;
	static const U8 defaultWidgetsBuffer = 6;
	bool emptySpots;
public:
	/** Construct a view.
	 *
	 * If you know how many widgets you add to the view pass the number of widgets+1.
	 *
	 * @param widgetsbuffer The starting objects buffer.
	 */
	NLcdView(U8 widgetsbuffer = defaultWidgetsBuffer) : widgets(widgetsbuffer), emptySpots(false) {}
	~NLcdView() {}

	/** \brief Add widget to view.
	 *
	 * Remove/add use memory efficient.
	 *
	 * @param myWidget Widget to add.
	 */
	void add(const NWidget *myWidget) {
		// search for empty spot
		if(emptySpots) {
			S32 idxOfEmpty = widgets.indexOf(static_cast<NWidget*>(NULL));
			if(idxOfEmpty != widgets.size()) {
				widgets.set(idxOfEmpty, myWidget);
				return;
			} else {
				emptySpots = false;
			}
		}
		widgets.pushBack(myWidget);
	}

	/** \brief Remove widget to view.
	 *
	 * Remove/add use memory efficient.
	 *
	 * @param myWidget Widget to add.
	 */
	void remove(const NWidget *myWidget) {
		// expensive and deletes the object
		// widgets.removeOne(&myWidget);
		S32 idxOfWidget = widgets.indexOf(myWidget);
		if(idxOfWidget != widgets.size()) {
			widgets.set(idxOfWidget, static_cast<NWidget*>(NULL));
			emptySpots = true;
		}
	}

	/** \brief Show all widgets.
	 *
	 * @param update If true update the display.
	 */
	void show(bool update = false) const {
		for(U16 i=0; i < widgets.size(); i++) {
			if(widgets[i] != static_cast<NWidget*>(NULL))
				widgets[i]->show();
		}
		if (update) {
			display_update();
		}
	}

	/** \brief Hide all widgets.
	 *
	 * @param update If true update the display.
	 */
	void hide(bool update = false) const {
		for(U16 i=0; i < widgets.size(); i++) {
			if(widgets[i] != static_cast<NWidget*>(NULL))
				widgets[i]->hide();
		}
		if (update) {
			display_update();
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
