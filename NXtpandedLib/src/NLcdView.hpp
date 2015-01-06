/*
 * NLcdView.hpp
 *
 *  Created on: 06.01.2015
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

/** \brief Group together NWidgets into view.
 *
 * A view is simply a class which manages the show/hide methods of multiple widgets.
 * You can for example implement page switching by changing the actual view.
 */
class NLcdView : public NGuiObject, private Uncopyable {
private:
	const NGuiObject** guiObjects;
	static const U8 defaultGuiObjectsBuffer = 6;
	int guiObjectsBufferSize;

	/** \brief Show all widgets.
	 *
	 * @param update If true update the display.
	 */
	virtual void showImpl() const;

	/** \brief Hide all widgets.
	 *
	 * @param update If true update the display.
	 */
	virtual void hideImpl() const;

public:
	/** Construct a view.
	 *
	 * If you know how many widgets you add to the view pass the number of widgets.
	 *
	 * @param guiObjectsbuffer The starting objects buffer.
	 */
	NLcdView(U8 guiObjectsbuffer = defaultGuiObjectsBuffer);

	~NLcdView()
	{
		delete[] guiObjects;
		guiObjects = 0;
	}

	/** \brief Add widget to view.
	 *
	 * If the buffer is full no object will be added.
	 *
	 * @param myWidget Widget to add.
	 */
	void add(const NGuiObject *myGuiObject);

	/** \brief Remove widget from view.
	 *
	 * Remove/add: use memory efficient.
	 *
	 * @param myWidget Widget to remove.
	 */
	void remove(const NGuiObject *myGuiObject);

	/** \brief Swap views to active/inactive.
	 *
	 * Use this if you have different views and you want to make one view active and one inactive.
	 *
	 * @param actualActive The actual active view (displayed on the screen)
	 * @param actualInactive The actual inactive view which should be now the active view.
	 * @param update If true update the display.
	 */
	static void swap(const NLcdView &actualActive, const NLcdView &actualInactive,
			bool update = false)
	{
		actualActive.hide();
		actualInactive.show(update);
	}
};

#endif /* __NLCDVIEW_HPP_ */
