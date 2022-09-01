TARGET_EXEC ?= main

BUILD_DIR ?= ./build
TARGET_DIR ?= ./bin
SRC_DIR ?= .

SRCS := $(shell find $(SRC_DIR) -name "*.cpp")
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

CPPFLAGS ?= -std=c++11 -MMD -MP

$(TARGET_DIR)/$(TARGET_EXEC): $(OBJS)
	$(MKDIR_P) $(dir $@)
	$(CXX) $(OBJS) -o $@

$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) -c $< -o $@

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR) $(TARGET_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
