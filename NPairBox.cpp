

#include "NPairBox.hpp"

namespace nxpl {

template<class T1, class T2>
bool NPairBox<T1, T2>::align2Main(const NAlignment align) {
	if (align == NAlignment::none()) {
		return true;
	}

	S8 adjustment = 0;
	if (align == NAlignment::right() || align == NAlignment::left()) {
		if (align == NAlignment::right())
			adjustment = align.get() + main->fieldWidth() - 1; //!
		else {
			adjustment = align.get() - sec->fieldWidth() - 1;
		}
		return sec->setPosition(main->indent() + adjustment, main->row());
	} else if (align == NAlignment::top() || align == NAlignment::bottom()) {
		adjustment = (align.get() > 0) ? (1) : (-1);
		return sec->setPosition(main->indent(), main->row() + adjustment);
	}
	return false;
}

}
