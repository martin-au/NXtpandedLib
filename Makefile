# Target specific macros
TARGET = mytest

TARGET_CPP_SOURCES := StringConvert.cpp \
					  NOstream.cpp \
				      NString.cpp \
					  sample.cpp 
					
			
	
TOPPERS_OSEK_OIL_SOURCE = ./sample.oil 

BUILD_MODE = ROM_ONLY

# GCC optimisation level
#-Os optimisation for size
#-O1/2/3 optimisation for speed
C_OPTIMISATION_FLAGS = -Os

# Don't modify below part
O_PATH ?= build

# makefile for C++(.cpp) build
include /nxtOSEK/ecrobot/ecrobot++.mak
