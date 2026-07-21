CC      ?= gcc
CFLAGS  ?= -std=c11 -Wall -Wextra -O2
LDFLAGS ?=

SRC_DIR   := src
INC_DIR   := include
BUILD_DIR := build
BIN_DIR   := bin

ifeq ($(OS),Windows_NT)
    MKDIR    = if not exist "$(1)" mkdir "$(1)"
    RMDIR    = if exist "$(1)" rmdir /s /q "$(1)"
    EXE_EXT  := .exe
    RUN_CMD  = $(subst /,\,$(TARGET))
else
    MKDIR    = mkdir -p "$(1)"
    RMDIR    = rm -rf "$(1)"
    EXE_EXT  :=
    RUN_CMD  = ./$(TARGET)
endif

LIB_SRCS := $(filter-out $(SRC_DIR)/main.c,$(wildcard $(SRC_DIR)/*.c))
LIB_OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(LIB_SRCS))

MAIN_SRC := $(SRC_DIR)/main.c
MAIN_OBJ := $(BUILD_DIR)/main.o

LIB_NAME := libdiscky.a
TARGET   := $(BIN_DIR)/discky_test$(EXE_EXT)

.PHONY: all lib test run clean

all: test

lib: $(BUILD_DIR)/$(LIB_NAME)

$(BUILD_DIR)/$(LIB_NAME): $(LIB_OBJS) | $(BUILD_DIR)
	ar rcs $@ $(LIB_OBJS)

test: $(TARGET)

$(TARGET): $(LIB_OBJS) $(MAIN_OBJ) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

run: test
	$(RUN_CMD)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

$(BUILD_DIR):
	$(call MKDIR,$(BUILD_DIR))

$(BIN_DIR):
	$(call MKDIR,$(BIN_DIR))

clean:
	$(call RMDIR,$(BUILD_DIR))
	$(call RMDIR,$(BIN_DIR))