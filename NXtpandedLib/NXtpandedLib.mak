
ifndef ROOT
ROOT := $(dir $(lastword $(MAKEFILE_LIST)))..
endif

NXTPANDEDLIB_SRC_PATH = $(ROOT)/NXtpandedLib/src

ifndef NXTPANDEDLIB_SOURCES
NXTPANDEDLIB_SOURCES = $(addprefix $(NXTPANDEDLIB_SRC_PATH)/,\
	StringConvert.cpp \
	NOstream.cpp \
	NString.cpp \
	NLabel.cpp \
	NPairBox.cpp \
	LcdDrawer.cpp	\
	NCom.cpp \
	NComSingle.cpp \
	Motorcontroller.cpp)
endif