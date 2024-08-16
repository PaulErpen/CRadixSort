# Compiler
CC = gcc

# Compiler flags
CFLAGS = -g -Wall -Wextra -Iinclude

# Directories
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Object files
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Target executable
TARGET = $(BUILD_DIR)/my_program

# Default rule: build the executable
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# Compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create the build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean up build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Phony targets
.PHONY: all clean
