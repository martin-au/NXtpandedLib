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

namespace nxpl {

/** \brief Make pair of two Widgets.
 *
 * This bundles two NWidgets which support the function setPosition(positioning in chars/lines);
 * You should bundle thing together which belongs together. For example two labels: One label shows a text
 * which describes the data of second label.
 * You have to set the position for only the main object or the box. The secondary object will be aligned with NAlignment to the main object.
 */
struct NPairBox : public NGuiObject {
private:
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
			main(mainWidget), sec(secWidget) {
	}

	/** \brief Constructor with positioning.
	 *
	 * @param mainWidget
	 * @param secWidget
	 * @param indent  Indent in chars from the left side of the display. (0 - 15)
	 * @param row  Row of the lcd (0 - 7)
	 * @param align The alignment of the secondary widget.
	 */
	NPairBox(NWidget *mainWidget, NWidget *secWidget, NCursor position, NAlignment align) :
			main(mainWidget), sec(secWidget) {
		setPosition(position, align);
	}

	/** \brief Constructor with aligning.
	 *
	 * @param mainWidget
	 * @param secWidget
	 * @param align The alignment of the secondary widget.
	 */
	NPairBox(NWidget *mainWidget, NWidget *secWidget, NAlignment align) :
			main(mainWidget), sec(secWidget) {
		align2Main(align);
	}

	~NPairBox() {}

	/** \brief Set position of the main object and align secondary object to main object.
	 *
	 * @param indent  Indent in chars from the left side of the display. (0 - 15)
	 * @param row     Row of the lcd (0 - 7)
	 * @param align   The alignment of the secondary widget.
	 * @return		  True if position is in lcd.
	 */
	void setPosition(NCursor position, NAlignment align) {
		main->setPosition(position);
		align2Main(align);
	}

	/** \brief Align secondary object to main object.
	 *
	 * @param align The alignment of the secondary widget.
	 * @return True if the box is in lcd.
	 */
	void align2Main(NAlignment align);
};

}

#endif /* __NPAIRBOX_HPP_ */
