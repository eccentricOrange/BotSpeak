# --- Directories ---
BUILD_DIR = build
SRC_DIR = src
INCLUDE_DIR = include
TEST_DIR = test

# --- Executables ---
LIB_NAME = BotSpeak
TEST_EXEC = test

# --- Paths ---
BOTSPEAK_SRC = $(SRC_DIR)/$(LIB_NAME).c
TEST_SRC = $(TEST_DIR)/$(TEST_EXEC).c

BOTSPEAK_OBJ = $(BUILD_DIR)/$(LIB_NAME).o
TEST_OBJ = $(BUILD_DIR)/$(TEST_EXEC).o

LIB_STATIC = $(BUILD_DIR)/lib$(LIB_NAME).a
TEST_BIN = $(BUILD_DIR)/$(TEST_EXEC)

# --- Compiler & Flags ---
CC = gcc
AR = ar
ARFLAGS = rcs
CFLAGS = -Wall -I$(INCLUDE_DIR)
LDFLAGS = -L$(BUILD_DIR) -l$(LIB_NAME) -lm

.PHONY: all clean lib test run-test

all: $(LIB_STATIC) $(TEST_BIN)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(BOTSPEAK_OBJ): $(BOTSPEAK_SRC) $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB_STATIC): $(BOTSPEAK_OBJ)
	$(AR) $(ARFLAGS) $@ $<

$(TEST_OBJ): $(TEST_SRC) $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_BIN): $(TEST_OBJ) $(LIB_STATIC)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

lib: $(LIB_STATIC)

test: $(TEST_BIN)

run-test: $(TEST_BIN)
	@$(TEST_BIN)

clean:
	@rm -rf $(BUILD_DIR)
