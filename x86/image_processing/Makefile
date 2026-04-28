# ASM 2023
# Makefile for image_processing
# Author : Bruno Da Rocha Carvalho

# Directories
#############
SRC_DIR := src
BUILD_DIR := build
INCLUDE_DIR := include
IMG_DIR := img

# Targets and source files
##########################
TARGET := $(BUILD_DIR)/image_processing
C_SOURCES := $(wildcard $(SRC_DIR)/*.c)
ASM_SOURCES := $(wildcard $(SRC_DIR)/*.S)
OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_SOURCES)) \
           $(patsubst $(SRC_DIR)/%.S,$(BUILD_DIR)/%.o,$(ASM_SOURCES))
DEPENDENCIES := $(OBJECTS:.o=.d)

# Flags
#######
ASFLAGS += -m32 -g -I$(INCLUDE_DIR) -Ilib
CFLAGS += -m32 -O0 -std=c11 -g -fverbose-asm -Wno-misleading-indentation -I$(INCLUDE_DIR) -Ilib
LDFLAGS += -m32 -Llib
LDLIBS += -lm

# Rules
#######
all: $(BUILD_DIR) $(DEPENDENCIES) $(TARGET) copy_images

# Create build directory if needed
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Rule to build the target
$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# Include dependency rules (only if not cleaning)
ifneq ($(MAKECMDGOALS),clean)
-include $(DEPENDENCIES)
endif

# Rule to compile C source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to compile Assembly source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.S | $(BUILD_DIR)
	$(CC) $(ASFLAGS) -c $< -o $@

# Rule to generate the dependency files
$(BUILD_DIR)/%.d: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -MM -MT '$(@:.d=.o)' $< -MF $@

$(BUILD_DIR)/%.d: $(SRC_DIR)/%.S | $(BUILD_DIR)
	$(CC) $(ASFLAGS) -MM -MT '$(@:.d=.o)' $< -MF $@

# Copy images directory to build
copy_images: $(BUILD_DIR)
	cp -r $(IMG_DIR) $(BUILD_DIR)/

# Clean rule
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean copy_images
