# ============================================================
#  Project Configuration
# ============================================================

# Name of the final binary
TARGET := roll

# Compiler and base flags
CC := gcc
CFLAGS := -Wall -Wextra -Werror -std=c17
LDFLAGS := -lm

# Optimization modes
DEBUG_FLAGS := -g -O0
RELEASE_FLAGS := -O2

# ============================================================
#  Directory Layout
# ============================================================

# Subdirectories containing sources and headers
SRC_DIRS := easyargs diceRoller simpleRNG formulaParser

# Object output and binary directories
OBJ_DIR := build
BIN_DIR := bin

# Collect all .c files in subdirectories
SRC := $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.c))

# Add main.c separately (ensures it's compiled and linked last)
MAIN_SRC := main.c

# Convert source files to corresponding object files
OBJ := $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC))
MAIN_OBJ := $(OBJ_DIR)/main.o

# Include paths (only for subfolders, not project root)
CFLAGS += $(foreach dir, $(SRC_DIRS), -I$(dir))

# ============================================================
#  Build Targets
# ============================================================

.PHONY: all debug release run clean help

all: debug

debug: CFLAGS += $(DEBUG_FLAGS)
debug: $(BIN_DIR)/$(TARGET)

release: CFLAGS += $(RELEASE_FLAGS)
release: $(BIN_DIR)/$(TARGET)

# ============================================================
#  Linking
# ============================================================

$(BIN_DIR)/$(TARGET): $(OBJ) $(MAIN_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $^ -o $@ $(LDFLAGS)
	@echo "Linked → $@"

# ============================================================
#  Compilation
# ============================================================

# Compile source files in subfolders
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -c $< -o $@
	@echo "Compiled → $<"

# Compile main.c (special case, not part of SRC_DIRS)
$(OBJ_DIR)/main.o: $(MAIN_SRC)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -c $< -o $@
	@echo "Compiled (main) → $<"

# Include auto-generated dependency files
-include $(OBJ:.o=.d) $(MAIN_OBJ:.o=.d)

# ============================================================
#  Utilities
# ============================================================

clean:
	@rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "Cleaned build artifacts."

run: all
	@echo "Running $(TARGET)..."
	@./$(BIN_DIR)/$(TARGET)#

install: all
	@echo "Compiling and installing..."
	@mv ./$(BIN_DIR)/$(TARGET) /usr/local/bin/roll
	@echo "Installed program at /usr/local/bin"
	@echo "You can now use the roll command from anywhere to roll dice"

help:
	@echo "Makefile commands:"
	@echo "  make            - Build in release mode"
	@echo "  make debug      - Build with debugging symbols"
	@echo "  make release    - Build optimized version"
	@echo "  make run        - Build and run"
	@echo "  make clean      - Remove all build artifacts"
	@echo "  make install    - Build and install into /usr/local/bin (requires sudo)"
