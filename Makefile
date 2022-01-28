CXX=arm-linux-gnueabi-g++
STRIP=arm-linux-gnueabi-strip
ROOT=.
#reorder 告警可能和编译屏障有关系,记得查资料
CFLAGS= -Wall -Wno-reorder -Wl,-Bdynamic -lsqlite3 -Wl,-Bstatic -g -Wno-unused-variable -static -lreadline -ltermcap -DNOSSL
TARGET=arm_linux_platform.elf
INC_DIRS=platform/include config/include/

SRC=$(wildcard *.cc) $(wildcard platform/src/*.cc) $(wildcard config/src/*.cc) \
	$(wildcard dev/fpga/*.cc) $(wildcard sqlite3/*.cc) $(wildcard app/*.cc) $(wildcard ftp/*.cc)
include config.mk
