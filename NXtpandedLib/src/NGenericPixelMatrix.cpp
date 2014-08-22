
#include "NGenericPixelMatrix.hpp"

namespace nxpl {

bool NGenericPixelMatrix::overlayOtherPixelMatrix(NGenericPixelMatrix matrix, bool transparent, U32 xPixel, U32 yRow) {
	if(!matrix.noError())
		return false;
	if(xPixel + matrix.getWidth() > this->getRows() || yRow + matrix.getRows() > this->getRows())
		return false;

	// copy full bitmap to display
	U32 iy; // in y
	U32 jx; // in x

	for (iy = yRow; iy < matrix.getRows(); iy++) { // for every line
		for (jx = xPixel; jx < matrix.getWidth(); jx++) {
			if(!transparent) {
				pixelMask(jx, iy, matrix.pixelMatrix[iy * matrix.getWidth() + jx]);
			}
			else {
				pixelMaskKeep(jx, iy, matrix.pixelMatrix[iy * matrix.getWidth() + jx]);
			}
		}
	}
	return true;
}

}
