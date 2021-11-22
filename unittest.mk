# Directory name
TST_DIR			:= test
UNITY_DIR		:= unity

# Directory root
TST_ROOT		:= $(PRJ_ROOT)/$(TST_DIR)

# Target definition
TARGET			:= $(BIN_ROOT)/unittest.elf

# Additional source files
SRCS			:= $(SRCS) \
	$(TST_ROOT)/$(UNITY_DIR)/unity.c \
	$(TST_ROOT)/test_main.c \
	$(TST_ROOT)/test_simple_averaged_buffer.c \

# Additional include directories
INC_DIRS		:= $(INC_DIRS) \
	$(TST_ROOT) \
	$(TST_ROOT)/$(UNITY_DIR) \

.PHONY: unittest
unittest: $(TARGET)

.PHONY: unittestclean
unittestclean:
	$(Q)$(RM) $(OBJS) $(DEPS) $(TARGET)

