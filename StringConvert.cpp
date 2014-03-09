
#include "StringConvert.hpp"

namespace nxpl {

const char _digitAsChar[] = "0123456789";

// dest must have 12 chars
S8 numToStr(S32 value, char* dest) {
	char *p = dest;
	U8 length = 0;

	if (value < 0) {
		*p++ = '-';
		value *= -1;
	}
	S32 shifter = value;
	do { // move to where representation ends
		++p;
		shifter = shifter / 10;
	} while (shifter);
	length = p - dest;
	*p = '\0';
	do { // move back, inserting digits as u go
		*--p = _digitAsChar[value % 10];
		value = value / 10;
	} while (value);
	return length;
}

S8 numToStr(U32 value, char* dest) {
	char *p = dest;
	U8 length = 0;

	U32 shifter = value;
	do { // move to where representation ends
		++p;
		length++;
		shifter = shifter / 10;
	} while (shifter);
	*p = '\0';
	do { // move back, inserting digits as u go
		*--p = _digitAsChar[value % 10];
		value = value / 10;
	} while (value);
	return length;
}

// for float to char*
typedef union {
	long l;
	float f;
} __lf_t;

S8 numToStr(float num, char* dest, U8 places) {
	S32 mantissa, int_part, frac_part;
	S16 exp2;
	__lf_t x;
	char *p;

	if (num == 0.0) {
		dest[0] = '0';
		dest[1] = '.';
		dest[2] = '0';
		dest[3] = '\0';
		return 3;
	}
	x.f = num;

	exp2 = (unsigned char) (x.l >> 23) - 127;
	mantissa = (x.l & 0xFFFFFF) | 0x800000;
	frac_part = 0;
	int_part = 0;

	if (exp2 >= 23)
		int_part = mantissa << (exp2 - 23);
	else if (exp2 >= 0) {
		int_part = mantissa >> (23 - exp2);
		frac_part = (mantissa << (exp2 + 1)) & 0xFFFFFF;
	} else
		// if (exp2 < 0)
		frac_part = (mantissa & 0xFFFFFF) >> -(exp2 + 1);

	p = &(dest[0]);

	if (x.l < 0) {
		*p++ = '-';
	}

	if (int_part == 0) {
		*p++ = '0';
	} else {
		numToStr(int_part, p);
		while (*p)
			p++;
	}
	*p++ = '.';

	if (frac_part == 0) {
		*p++ = '0';
	} else {
		char m;
		for (m = 0; m < places; m++) {
			frac_part = (frac_part << 3) + (frac_part << 1);

			*p++ = (frac_part >> 24) + '0';
			frac_part &= 0xFFFFFF;
		}
		// delete ending zeroes
		for (--p; p[0] == '0' && p[-1] != '.'; --p)
			;
		++p;
	}
	*p = 0;
	return (p - dest);
}

// dest must have 8 chars
S8 numToHex(U32 value, char* dest) {
	static char const digit[] = "0123456789ABCDEF";
	char *p = dest;
	U32 length = 0;

	S32 shifter = value;
	do { // move to where representation ends
		++p;
		length++;
		shifter = shifter / 16;
	} while (shifter);
	*p = '\0';
	do { // move back, inserting digits as u go
		*--p = digit[value % 16];
		value = value / 16;
	} while (value);
	return length;
}

}
