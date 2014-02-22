/*
 * NLcdView.hpp
 *
 *  Created on: 22.02.2014
 *      Author: Martin
 */

#ifndef __NLCDVIEW_HPP_
#define __NLCDVIEW_HPP_

#include "NVector.hpp"
#include "NWidget.hpp"
#include "Uncopyable.hpp"

namespace nxpl {

class NLcdView : private Uncopyable {
private:
	NVector<const NWidget*> widgets;
	static const U8 defaultWidgetsBuffer = 6;
public:
	NLcdView() : widgets(defaultWidgetsBuffer) {}
	~NLcdView() {}

	void add(const NWidget *myWidget) {
		widgets.pushBack(myWidget);
	}
	void remove(const NWidget *myWidget) {
		// expensive and deletes the object
		// widgets.removeOne(&myWidget);
		S32 idxOfWidget = widgets.indexOf(myWidget);
		if(idxOfWidget != widgets.size()) {
			widgets.set(idxOfWidget, static_cast<NWidget*>(NULL));
		}
	}

	void show(bool update = false) const {
		for(U16 i=0; i < widgets.size(); i++) {
			if(widgets[i] != static_cast<NWidget*>(NULL))
				widgets[i]->show();
		}
		if (update) {
			display_update();
		}
	}

	void hide(bool update = false) const {
		for(U16 i=0; i < widgets.size(); i++) {
			if(widgets[i] != static_cast<NWidget*>(NULL))
				widgets[i]->hide();
		}
		if (update) {
			display_update();
		}
	}

	static void swap(const NLcdView &actualActive, const NLcdView &actualInactive,
			bool update = false) {
		actualActive.hide();
		actualInactive.show(update);
	}
};


}

#endif /* __NLCDVIEW_HPP_ */
