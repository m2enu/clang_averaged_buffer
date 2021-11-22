MAKEFLAGS		+= --no-builtin-rules

# Directory name
SRC_DIR			:= src
OBJ_DIR			:= obj
BIN_DIR			:= bin

# Directory root
PRJ_ROOT		:= $(abspath .)
SRC_ROOT		:= $(PRJ_ROOT)/$(SRC_DIR)
OBJ_ROOT		:= $(PRJ_ROOT)/$(OBJ_DIR)
BIN_ROOT		:= $(PRJ_ROOT)/$(BIN_DIR)

# Source files
SRCS			:= \
	$(SRC_ROOT)/iaveraged_buffer.c \
	$(SRC_ROOT)/simple_averaged_buffer.c \

# Include directories
INC_DIRS        := \
	$(SRC_ROOT) \

# Toolchain
CC				:= gcc
RM				:= rm -rf
MKDIR			:= mkdir -p

# Compiler option
CFLAGS			:= \
	-Wall \
	-Wextra \
	-MMD \
	-MP \

# Linker option
LDFLAGS         := \

# Additional configuration
IS_UNITTEST_1 := $(findstring unittest,$(MAKEFLAGS))
IS_UNITTEST_2 := $(findstring unittest,$(MAKECMDGOALS))
ifeq ($(or $(IS_UNITTEST_1),$(IS_UNITTEST_2)),)
	-include all.mk
else
	-include unittest.mk
endif

# Definition
INCLUDE			:= $(foreach dir,$(INC_DIRS),-I$(dir))
OBJS			:= $(subst $(PRJ_ROOT),$(OBJ_ROOT),$(SRCS:.c=.o))
DEPS			:= $(OBJS:.o=.d)

# Quiet
ifeq ($(VERBOSE),1)
Q				:=
else
Q				:= @
endif

.PHONY: all
all: $(TARGET)

.PHONY: clean
clean:
	$(Q)$(RM) $(OBJS) $(DEPS) $(TARGET)

$(TARGET): $(OBJS)
	@[ -d `dirname $@` ] || $(MKDIR) `dirname $@`
	$(info Generating $(notdir $@))
	$(Q)$(CC) $(LDFLAGS) -o $@ $^

$(OBJ_ROOT)/%.o: $(PRJ_ROOT)/%.c
	@[ -d `dirname $@` ] || $(MKDIR) `dirname $@`
	$(info Compiling $(notdir $<))
	$(Q)$(CC) $(CFLAGS) $(INCLUDE) -o $@ -c $<

IS_DRYRUN := $(findstring n,$(filter-out --%,$(MAKEFLAGS)))
ONLY_CLEAN := $(findstring _clean_,_$(MAKEFLAGS)_)
ifeq ($(or $(IS_DRYRUN),$(ONLY_CLEAN)),)
	-include $(DEPS)
endif

# end of file {{{1
# vim:ft=make:noet:ts=4:nowrap:fdm=marker
