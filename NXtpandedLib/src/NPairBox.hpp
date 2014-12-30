/*
 * NPairBox.hpp
 *
 *  Created on: 25.03.2014
 *      Author: Martin
 */

#ifndef __NPAIRBOX_HPP_
#define __NPAIRBOX_HPP_

/** \file
 *	\ingroup NxtLcd
*/

//#include <C:/cygwin/GNUARM/include/c++/4.0.2/tr1/utility>

#include "NAlignment.hpp"
#include "NWidget.hpp"
#include "NCursor.hpp"

/** \brief Make pair of two Widgets.
 *
 * This bundles two NWidgets which support the function setPosition(positioning in chars/lines);
 * You should bundle thing together which belongs together. For example two labels: One label shows a text
 * which describes the data of second label.
 * You have to set the position for only the main object or the box. The secondary object will be aligned with NAlignment to the main object.
 */
struct NPairBox : public NGuiObject {
private:
	NAlignment alignment;

	virtual void showImpl() const {
		main->show();
		sec->show();
	}
	virtual void hideImpl() const {
		main->hide();
		sec->hide();
	}

public:
	NWidget* main; /**<Main object*/
	NWidget* sec;  /**<Secondary object*/

	/** \brief Constructor without positioning.
	 *
	 * @param mainWidget
	 * @param secWidget
	 */
	NPairBox(NWidget *mainWidget, NWidget *secWidget) :
		alignment(NAlignment::none()), main(mainWidget), sec(secWidget) {
	}

	/** \brief Constructor with positioning and aligning.
	 *
	 * @param mainWidget
	 * @param secWidget
	 * @param position  Set position of the main object and align secondary object to main object.
	 * @param align The alignment of the secondary widget.
	 */
	NPairBox(NWidget *mainWidget, NWidget *secWidget, NCursor position, NAlignment align) :
		alignment(align), main(mainWidget), sec(secWidget) {
		setPosition(position);
	}

	/** \brief Constructor with aligning.
	 *
	 * @param mainWidget
	 * @param secWidget
	 * @param align The alignment of the secondary widget.
	 */
	NPairBox(NWidget *mainWidget, NWidget *secWidget, NAlignment align) :
		alignment(align), main(mainWidget), sec(secWidget) {
		align2Main(align);
	}

	virtual ~NPairBox() {}

	/** \brief Set position of the main object and align secondary object to main object.
	 *
	 * @param position  Position of the pair widget.
	 * @return		    True if position is in lcd.
	 */
	void setPosition(NCursor position) {
		main->setPosition(position);
		align2Main(alignment);
	}

	/** \brief Align secondary object to main object.
	 *
	 * @param align The alignment of the secondary widget.
	 * @return True if the box is in lcd.
	 */
	void align2Main(NAlignment align);
};

#endif /* __NPAIRBOX_HPP_ */
