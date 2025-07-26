# Define build directory
BUILD_DIR = bin

# Compiler commands and flags
CC = gcc
CXX = g++
CFLAGS = -Iinclude -Wall -Wextra -g
CXXFLAGS = -Iinclude -Wall -Wextra -g

# Source files
BOT_SPEAK_SRC = src/bot_speak_frame_ops.c
TEST_FRAME_OPS_SRC = test/test_frame_ops.c
TEST_REAL_WORLD_SRC = test/test_real_world.cpp

# Output names
BUILD_DIR_TASK = mkdir_build
BOT_SPEAK_LIB = $(BUILD_DIR)/libbotspeak.a
TEST_FRAME_OPS_BIN = $(BUILD_DIR)/test_frame_ops
TEST_REAL_WORLD_BIN = $(BUILD_DIR)/test_real_world

# Create the build directory if it doesn't exist
$(BUILD_DIR_TASK):
	@mkdir -p $(BUILD_DIR)

# .PHONY: all build lib test_frame_ops test_real_world run_test_frame_ops run_test_real_world clean
.PHONY: all build lib test_frame_ops run_test_frame_ops clean

# Default target: alias for 'build'
all: build

# Alias for 'lib'
build: lib

# Compile botSpeak C source into a static library
# Dependencies: Requires the build directory to exist
lib: $(BUILD_DIR_TASK) $(BOT_SPEAK_SRC)
	$(CC) $(CFLAGS) -c $(BOT_SPEAK_SRC) -o $(BUILD_DIR)/bot_speak_frame_ops.o
	ar rcs $(BOT_SPEAK_LIB) $(BUILD_DIR)/bot_speak_frame_ops.o

# Compile test/test_frame_ops.c with botSpeak library dependency
# Dependencies: Requires the botSpeak library to be built and the build directory to exist
test_frame_ops: lib $(BUILD_DIR_TASK) $(BOT_SPEAK_LIB) $(TEST_FRAME_OPS_SRC)
	$(CC) $(CFLAGS) $(TEST_FRAME_OPS_SRC) $(BOT_SPEAK_LIB) -o $(TEST_FRAME_OPS_BIN)

# Compile test/test_real_world.cpp with botSpeak library and libserial dependencies
# Dependencies: Requires the botSpeak library to be built and the build directory to exist
# test_real_world: lib $(BUILD_DIR_TASK) $(BOT_SPEAK_LIB) $(TEST_REAL_WORLD_SRC)
# 	$(CXX) $(CXXFLAGS) $(TEST_REAL_WORLD_SRC) $(BOT_SPEAK_LIB) -lserial -o $(TEST_REAL_WORLD_BIN)

# Run and compile test_frame_ops
run_test_frame_ops: test_frame_ops
	$(TEST_FRAME_OPS_BIN)

# Run and compile test_real_world
# run_test_real_world: test_real_world
# 	$(TEST_REAL_WORLD_BIN)

# Clean up build artifacts
clean:
	rm -rf $(BUILD_DIR)
