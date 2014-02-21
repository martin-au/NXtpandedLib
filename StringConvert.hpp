
#ifndef __CONVERT_H_
#define __CONVERT_H_


extern "C" {
#include "C:/cygwin/nxtOSEK/toppers_osek/include/kernel.h"
#include "kernel_id.h"
#include "C:/cygwin/nxtOSEK/ecrobot/c/ecrobot_interface.h"
}

#include "C:/cygwin/GNUARM/include/c++/4.0.2/tr1/type_traits"

namespace nxpl {

// the functions should be used to calculate the resulting string size before
// actual converting. Remember to let space for the '\0' char!!
inline S8 numDigits(S32 num) {
	S8 cnt= (num < 0) ? 1 : 0;
	for(; num; num /= 10, cnt++);
	return cnt;
}

inline S8 numDigits(U32 num) {
	S8 cnt= 0;
	for(; num; num /= 10, cnt++);
	return cnt;
}

//convert helper
S8 numToStr(S32 value, char* dest);
S8 numToStr(U32 value, char* dest);
S8 numToStr(float f, char* dest, U8 places = 2);
S8 numToHex(U32 value, char* dest);

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

}

#endif // __CONVERT_H_