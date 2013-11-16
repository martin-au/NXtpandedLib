
#ifndef __CONVERT_H_
#define __CONVERT_H_


extern "C" {
#include "C:/cygwin/nxtOSEK/toppers_osek/include/kernel.h"
#include "kernel_id.h"
#include "C:/cygwin/nxtOSEK/ecrobot/c/ecrobot_interface.h"
}

//convert helper
U32 numToStr(S32 value, char* dest);
U32 numToStr(U32 value, char* dest);
/// needs lot of memory
U32 numToStr(float value, char *dest, U16 places);
U32 numToHex(U32 value, char* dest);

// all other
// not for unsigned float!
template<typename T>
U32 numToStr(T value, char* dest) {
	if (value < 0)
		return numToStr(static_cast<S32>(value), dest);
	else
		return numToStr(static_cast<U32>(value), dest);
}

// easier
#include "convert.cpp"

#endif // __CONVERT_H_
