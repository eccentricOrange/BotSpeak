# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I./include
LDFLAGS =

# Directories
BUILD_DIR = ./build
SRC_DIR = ./src
TEST_DIR = ./test
INC_DIR = ./include

# Library details
LIB_NAME = bot_speak
LIB_FILE = $(BUILD_DIR)/lib$(LIB_NAME).a
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

# Test details
TEST_SRC = $(TEST_DIR)/test_frame_ops.c
TEST_EXEC = $(BUILD_DIR)/test_frame_ops

.PHONY: all build test run_test clean

# Default target: builds library, test, and runs the test
all: run_test

# Create build directory if it doesn't exist
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# -----------------------------------------------------------------------------
# Command to build the library itself (now aliased as 'build')
# -----------------------------------------------------------------------------
build: $(LIB_FILE)

$(LIB_FILE): $(OBJ_FILES) $(BUILD_DIR)
	@echo "Building library: $(LIB_FILE)"
	ar rcs $(LIB_FILE) $(OBJ_FILES)
	@echo "Library built successfully."

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# -----------------------------------------------------------------------------
# Command to build the test (with the library)
# -----------------------------------------------------------------------------
test: $(TEST_EXEC)

$(TEST_EXEC): $(TEST_SRC) $(LIB_FILE) $(BUILD_DIR)
	@echo "Building test executable: $(TEST_EXEC)"
	$(CC) $(CFLAGS) $(TEST_SRC) $(LIB_FILE) -o $@ $(LDFLAGS)
	@echo "Test executable built successfully."

# -----------------------------------------------------------------------------
# Command to run the test (with building the test)
# -----------------------------------------------------------------------------
run_test: test
	@echo "Running tests..."
	@$(TEST_EXEC)

# Clean up build artifacts
clean:
	@echo "Cleaning up build directory..."
	@rm -rf $(BUILD_DIR)
	@echo "Clean complete."

