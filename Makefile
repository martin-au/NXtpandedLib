# Target specific macros
TARGET = mytest

TARGET_CPP_SOURCES = sample.cpp
	
TOPPERS_OSEK_OIL_SOURCE = ./sample.oil 

# Don't modify below part
O_PATH ?= build

# makefile for C++(.cpp) build
include /nxtOSEK/ecrobot/ecrobot++.mak
