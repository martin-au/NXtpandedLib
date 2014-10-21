
ifndef ROOT
ROOT := $(dir $(lastword $(MAKEFILE_LIST)))..
endif

NXTPANDEDLIB_SRC_PATH = $(ROOT)/NXtpandedLib/src

ifndef NXTPANDEDLIB_SOURCES
NXTPANDEDLIB_SOURCES = $(addprefix $(NXTPANDEDLIB_SRC_PATH)/,\
		StringConvert.cpp \
		NString.cpp \
		NOstream.cpp \
		NLabel.cpp \
		NPairBox.cpp \
		NCom.cpp \
		NComSingle.cpp \
		Motorcontroller.cpp \
		NGenericPixelMatrix.cpp \
		NLine.cpp \
		NRectangle.cpp \
		NCircle.cpp \
		NEllipse.cpp \
		)
endif