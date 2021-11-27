.PHONY:all clean

SHELL:=/bin/bash
MKDIR = mkdir -p
RM = rm
RMFLAGS = -rf

ARFLAGS = crs
DLFLAGS = -fPIC -shared

DIR_OBJS = $(ROOT)/objs
DIR_ELF = $(ROOT)/target
DIR_LIBS = $(ROOT)/libs
DIR_DLIBS = $(ROOT)/dlibs

DIRS = $(DIR_ELF) $(DIR_LIBS) $(DIR_DLIBS)

ifeq ($(CXX),g++)
CXX=g++
AR=ar
STRIP=strip
endif

ifeq ($(VERSION),"")
VERSION="plaform-name-version"
endif

ifdef TARGET
TARGET :=$(addprefix $(DIR_ELF)/,$(TARGET))
endif

ifdef LIB
LIB := $(addprefix $(DIR_LIBS)/,$(LIB))
endif

ifdef DLIB
DLIB := $(addprefix $(DIR_DLIBS)/,$(DLIB))
endif

#单独指定可以编译的源文件
ifndef SRC
SRCS=$(wildcard *.cc)
OBJS=$(SRCS:.cc=.o)
OBJS :=$(addprefix $(DIR_OBJS)/,$(OBJS))
else
SRCS:=$(SRC)
OBJS=$(SRCS:.cc=.o)
OBJS :=$(addprefix $(DIR_OBJS)/,$(OBJS))
endif

ifndef STATIC_OBJS
STATIC_OBJS = $(OBJS)
else
STATIC_OBJS := $(STATIC_OBJS:.cc=.o)
STATIC_OBJS := $(addprefix $(DIR_OBJS)/,$(STATIC_OBJS))
endif

ifneq ($(INC_DIRS),"")
INC_DIRS := $(strip $(INC_DIRS))
INC_DIRS :=$(addprefix -I,$(INC_DIRS))
endif

ifneq ($(LINK_LIBS),"")
LINK_LIBS := $(strip $(LINK_LIBS))
LINK_LIBS := $(addprefix -l,$(LINK_LIBS))
endif

ifneq ($(LINK_DLIBS),"")
LINK_DLIBS := $(strip $(LINK_DLIBS))
LINK_DLIBS := $(addprefix -l,$(LINK_DLIBS))
endif

define MAKE_EXTERN
@set -e ;\
	for tmp in $(1) ;\
	do \
	make -C $$tmp ;\
	done
endef

define MAKE_EXTERN_CLEAN
@set -e ;\
	for tmp in $(1) ;\
	do \
	make clean -C $$tmp ;\
	done
endef

#################### MAIN #####################
all:
	make $(DIRS)
	make $(DIR_OBJS)
	make version
ifdef LIB
	make $(LIB)
endif
ifdef DLIB
	make $(DLIB)
endif
ifdef MAKE_PRE
	$(call MAKE_EXTERN,$(MAKE_PRE))
endif
ifdef TARGET
	make $(TARGET)
endif
ifdef MAKE_OTHER
	$(call MAKE_EXTERN,$(MAKE_OTHER))
endif
#################### MAIN #####################

$(DIRS):
	$(MKDIR) $@

$(DIR_OBJS):
	$(MKDIR) $(addprefix $(DIR_OBJS)/,$(dir $(SRC))) $(dir $(STATIC_OBJS))

define COLOR_PATH
echo -e "\e[31;1m$(1) $(2) $(3)\033[0m"
endef

define ECHO_APP_PATH
	@set -e ;\
	[[ -f $(1) ]] && $(call COLOR_PATH,[ NEW APP PATH ],>>>>>>,$(abspath $(1))) ;\
	[[ -f $(1) ]] && echo "$$(date) $$(md5sum $(abspath $(1)))" >> version.log
endef

$(TARGET):$(OBJS)
	$(CXX) -L$(DIR_LIBS) -L$(DIR_DLIBS) $(filter %.o,$^) $(LINK_LIBS) $(LINK_DLIBS) -o $@ $(CFLAGS) $(INC_DIRS)
	$(STRIP) $(TARGET)
	$(call ECHO_APP_PATH,$@)

$(LIB): $(STATIC_OBJS)
	$(AR) $(ARFLAGS) $@ $(filter %.o,$^)

$(DLIB): $(SRCS)
	$(CXX) $(DLFLAGS) $(filter %.cc,$^)  -o $@ $(CFLAGS) $(INC_DIRS)

$(DIR_OBJS)/%.o : %.cc
	$(CXX) $(INC_DIRS) -E -MM $(filter %.cc,$^) > $@.dep
	$(CXX) $(INC_DIRS) -o $@ -c $(filter %.cc,$^) $(CFLAGS) $(INC_DIRS)

tags:
	@ctags -R ;\
		cscope -Rbq
dist:
	@tar cvzf $$(date +%y%m%d%H%M).tar.gz $(TARGET)

version:
	echo "#ifndef __CONFIG_HEAD_VERSION" > config.hpp
	echo "" >> config.hpp
	echo "#endif /*__CONFIG_HEAD_VERSION*/" >> config.hpp
	sed  -i 2c"static const char platform_version[] =\"$(VERSION)\";" config.hpp

clean:
ifdef MAKE_OTHER
	$(call MAKE_EXTERN_CLEAN,$(MAKE_OTHER))
endif
ifdef MAKE_PRE
	$(call MAKE_EXTERN_CLEAN,$(MAKE_PRE))
endif
	$(RM) $(RMFLAGS) $(notdir $(DIRS)) objs $(OBJS) tags cscope* config.hpp