# Simple Makefile using sdl-config for SDL flags
CC := gcc
SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LIBS := $(shell sdl2-config --libs)
CFLAGS := -Wall -Wextra -std=c99 $(SDL_CFLAGS)
SRCDIR := src
SOURCES := $(wildcard $(SRCDIR)/*.c)
BUILD_DIR := build
OBJECTS := $(patsubst $(SRCDIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))
TARGET := star-invaders

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@echo " Linking $@"
	@$(CC) $(OBJECTS) -o $@ $(SDL_LIBS)

$(BUILD_DIR)/%.o: $(SRCDIR)/%.c | $(BUILD_DIR)
	@echo " Compiling $<"
	@$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

clean:
	@echo "Cleaning up"
	@rm -rf $(BUILD_DIR) $(TARGET)
