# -----------------------------------------------------------------------------
# Configuration
# -----------------------------------------------------------------------------

# Shell and Compiler Settings
SHELL = bash
ifdef OS
SHELL = C:/Program Files/Git/bin/bash.exe
endif
CC = gcc
DEBUGCC = g++
CFLAGS = -std=c17 -pedantic -O2 -Wall
DEBUG_CFLAGS = -std=c17 -pedantic -g -Wall -Wextra -DDEBUG

# Common Commands
MKDIR = mkdir -p
RMDIR = rm -rf
OPEN = open
ifdef OS
OPEN = start
endif

# -----------------------------------------------------------------------------
# Project Directories and Files
# -----------------------------------------------------------------------------

COMPILER_NAME = lsharpc

# Directories
COMPILER_SRC_DIR = src/compiler
BUILD_DIR = build
PUBLISH_DIR = bin
DOCS_DIR = docs
TESTS_DIR = tests
TESTS_BUILD_DIR = $(BUILD_DIR)/tests

# Executables
COMPILER = $(PUBLISH_DIR)/$(COMPILER_NAME).exe
COMPILER_DEBUG = $(PUBLISH_DIR)/$(COMPILER_NAME)-debug.exe
COMPILER_TESTS = $(PUBLISH_DIR)/$(COMPILER_NAME)-tests.exe

# Source Files and Objects
COMPILER_HEADERS = $(shell find $(COMPILER_SRC_DIR) -name "*.h")
COMPILER_SOURCES = $(shell find $(COMPILER_SRC_DIR) -name "*.c")
TEST_DEPS = $(shell find $(COMPILER_SRC_DIR)/*/ -name "*.c")
TEST_FILES = $(shell find $(TESTS_DIR) -name "*.c")
COMPILER_OBJS = $(patsubst $(COMPILER_SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(COMPILER_SOURCES))
COMPILER_DEBUG_OBJS = $(patsubst $(COMPILER_SRC_DIR)/%.c,$(BUILD_DIR)/debug/%.o,$(COMPILER_SOURCES))
TEST_OBJS = $(patsubst $(TESTS_DIR)/%.c,$(TESTS_BUILD_DIR)/%.o,$(TEST_FILES)) $(patsubst $(COMPILER_SRC_DIR)/%.c,$(TESTS_BUILD_DIR)/%.o,$(TEST_DEPS))

# -----------------------------------------------------------------------------
# Build Rules
# -----------------------------------------------------------------------------

.PHONY: all
all: $(COMPILER)

$(BUILD_DIR)/%.o: $(COMPILER_SRC_DIR)/%.c $(COMPILER_HEADERS)
	$(MKDIR) "$(dir $@)"
	$(CC) $(CFLAGS) -c $< -o $@

$(COMPILER): make_build_paths $(COMPILER_OBJS)
	$(CC) $(COMPILER_OBJS) $(CFLAGS) -o $@

# -----------------------------------------------------------------------------
# Debug Rules
# -----------------------------------------------------------------------------
.PHONY: debug
debug: $(COMPILER_DEBUG)

$(BUILD_DIR)/debug/%.o: $(COMPILER_SRC_DIR)/%.c $(COMPILER_HEADERS)
	$(MKDIR) "$(dir $@)"
	$(DEBUGCC) $(DEBUG_CFLAGS) -c $< -o $@

$(COMPILER_DEBUG): make_build_paths $(COMPILER_DEBUG_OBJS)
	$(DEBUGCC) -fdiagnostics-color=always -g $(COMPILER_DEBUG_OBJS) -o $@

# -----------------------------------------------------------------------------
# Testing Rules
# -----------------------------------------------------------------------------

PHONY: tests
tests: $(COMPILER_TESTS)

$(TESTS_BUILD_DIR)/%.o: $(TESTS_DIR)/%.c $(COMPILER_HEADERS)
	$(MKDIR) "$(dir $@)"
	$(CC) $(CFLAGS) -c $< -o $@

$(TESTS_BUILD_DIR)/%.o: $(COMPILER_SRC_DIR)/%.c $(COMPILER_HEADERS)
	$(MKDIR) "$(dir $@)"
	$(CC) $(CFLAGS) -c $< -o $@

$(COMPILER_TESTS): make_tests_paths $(TEST_OBJS)
	$(CC) $(TEST_OBJS) $(CFLAGS) -o $@
	$(COMPILER_TESTS)

# -----------------------------------------------------------------------------
# Directory Creation
# -----------------------------------------------------------------------------

make_build_paths:
	$(MKDIR) "$(BUILD_DIR)"
	$(MKDIR) "$(PUBLISH_DIR)"

make_tests_paths:
	$(MKDIR) "$(TESTS_BUILD_DIR)"
	$(MKDIR) "$(PUBLISH_DIR)"

make_docs_paths:
	$(MKDIR) "$(DOCS_DIR)"

# -----------------------------------------------------------------------------
# Documentation
# -----------------------------------------------------------------------------

PHONY: docs
docs: make_docs_paths $(COMPILER) $(COMPILER_OBJS)
	@doxygen
	@$(OPEN) "$(DOCS_DIR)/html/index.html"

# -----------------------------------------------------------------------------
# Cleanup
# -----------------------------------------------------------------------------

PHONY: clean clean_build clean_tests clean_docs
clean: clean_tests clean_docs clean_build

clean_build:
	$(RMDIR) "$(BUILD_DIR)"
	$(RMDIR) "$(PUBLISH_DIR)"

clean_tests:
	$(RMDIR) "$(TESTS_BUILD_DIR)"

clean_docs:
	$(RMDIR) "$(DOCS_DIR)"

# -----------------------------------------------------------------------------
# Rebuild and Retest
# -----------------------------------------------------------------------------

PHONY: rebuild retest
rebuild: clean all
retest: clean_tests tests