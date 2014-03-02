/*
 * PairBox.hpp
 *
 *  Created on: 12.02.2014
 *      Author: Martin
 */

#ifndef __NPAIRBOX_HPP_
#define __NPAIRBOX_HPP_

/** \file
 *	\ingroup NxtLcd
*/

#include <C:/cygwin/GNUARM/include/c++/4.0.2/tr1/utility>

#include "NAlignment.hpp"

namespace nxpl {

/** \brief Make pair of two Widgets.
 *
 * This bundles two NWidgets which support the function setPosition(positioning in chars/lines);
 * You should bundle thing together which belongs together. For example two labels: One label shows a text
 * which describes the data of second label.
 * You have to set the position for only the main object or the box. The secondary object will be aligned with NAlignment to the main object.
 */
template<class T1, class T2>
struct NPairBox {
	T1* main; /**<Main object*/
	T2* sec;  /**<Secondary object*/

	/** \brief Constructor without positioning.
	 *
	 * @param mainWidget
	 * @param secWidget
	 */
	NPairBox(T1 *mainWidget, T2 *secWidget) :
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
	NPairBox(T1 *mainWidget, T2 *secWidget, S8 indent, S8 row, const NAlignment align) :
			main(mainWidget), sec(secWidget) {
		setPosition(indent, row, align);
	}

	/** \brief Constructor with aligning.
	 *
	 * @param mainWidget
	 * @param secWidget
	 * @param align The alignment of the secondary widget.
	 */
	NPairBox(T1 *mainWidget, T2 *secWidget, const NAlignment align) :
			main(mainWidget), sec(secWidget) {
		align2Main(align);
	}

	~NPairBox() {}

	/** \brief Make the box visible with both objects.
	 *
	 * @param update If true update the display.
	 */
	void show(bool update = false) const {
		main->show(false);
		sec->show(update);
	}

	/** \brief Make the box invisible with both objects.
	 *
	 * @param update If true update the display.
	 */
	void hide(bool update = false) const {
		main->hide(false);
		sec->hide(update);
	}

	/** \brief Set position of the main object and aling secondary object to main object.
	 *
	 * @param indent  Indent in chars from the left side of the display. (0 - 15)
	 * @param row     Row of the lcd (0 - 7)
	 * @param align   The alignment of the secondary widget.
	 * @return		  True if position is in lcd.
	 */
	bool setPosition(S8 indent, S8 row, const NAlignment align) {
		if(!main->setPosition(indent, row)) {
			return false;
		}
		return align2Main(align);
	}

	/** \brief Align secondary object to main object.
	 *
	 * @param align The alignment of the secondary widget.
	 * @return True if the box is in lcd.
	 */
	bool align2Main(const NAlignment align);
};

}

#include "NPairBox.cpp"

#endif /* __NPAIRBOX_HPP_ */
