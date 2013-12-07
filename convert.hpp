
#ifndef __CONVERT_H_
#define __CONVERT_H_


extern "C" {
#include "C:/cygwin/nxtOSEK/toppers_osek/include/kernel.h"
#include "kernel_id.h"
#include "C:/cygwin/nxtOSEK/ecrobot/c/ecrobot_interface.h"
}

#include "C:/cygwin/GNUARM/include/c++/4.0.2/tr1/type_traits"

//convert helper
S8 numToStr(S32 value, char* dest);
S8 numToStr(U32 value, char* dest);
S8 numToStr(float f, char* dest, U8 places = 2);
S8 numToHex(U32 value, char* dest);

// all other
// not for unsigned float!
template<typename T>
S8 numToStr(T value, char* dest) {
	if(std::tr1::is_floating_point<T>::value) {
		return numToStr(static_cast<float>(value), dest);
	}
	if (std::tr1::is_signed<T>::value)
		return numToStr(static_cast<S32>(value), dest);
	else
		return numToStr(static_cast<U32>(value), dest);
}

// easier
#include "convert.cpp"

#endif // __CONVERT_H_
