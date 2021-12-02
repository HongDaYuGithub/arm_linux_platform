CXX=g++
ROOT=.
#reorder 告警可能和编译屏障有关系,记得查资料
CFLAGS= -Wall -Wno-reorder -g -lsqlite3 -g -Wno-unused-variable
TARGET=cplus_debug
INC_DIRS=platform/include config/include/

SRC=$(wildcard *.cc) $(wildcard platform/src/*.cc) $(wildcard config/src/*.cc) \
	$(wildcard dev/fpga/*.cc) $(wildcard sqlite3/*.cc) $(wildcard app/*.cc)
include config.mk