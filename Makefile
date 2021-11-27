CXX=g++
ROOT=.
CFLAGS= -Wall -g
TARGET=cplus_debug
INC_DIRS=platform/include config/include/

SRC=$(wildcard *.cc) $(wildcard platform/src/*.cc) $(wildcard config/src/*.cc) \
	$(wildcard dev/*.cc)
include config.mk