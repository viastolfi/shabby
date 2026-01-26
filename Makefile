SRC = src
BUILD = build
RAYLIB_FOLDER = raylib-5.5_linux_amd64

CPPS = \
	$(SRC)/main.cpp \
	$(SRC)/engine/entities/entities.cpp \
	$(SRC)/engine/entities/entity_manager.cpp \
	$(SRC)/engine/core/engine.cpp \
	$(SRC)/engine/core/sprite/sprite.cpp \
	$(SRC)/game/player.cpp

OBJ = \
	$(BUILD)/main.o \
	$(BUILD)/engine/entities/entities.o \
	$(BUILD)/engine/entities/entity_manager.o \
	$(BUILD)/engine/core/engine.o \
	$(BUILD)/engine/core/sprite/sprite.o \
	$(BUILD)/game/player.o

CXX = g++

CXXFLAGS = -std=c++17 -Wall -Wextra -g -Isrc

RAYLIB_INC = -isystem $(RAYLIB_FOLDER)/include
RAYLIB_LIB = -L$(RAYLIB_FOLDER)/lib -l:libraylib.a -lm

UNAME_S := $(shell uname -s)
UNAME_M := $(shell uname -m)

.PHONY: all clean check-platform

all: check-platform $(BUILD)/shabby

$(BUILD)/shabby: $(OBJ)
	$(CXX) -o $@ $^ $(RAYLIB_LIB)

$(BUILD)/%.o: $(SRC)/%.cpp
	@mkdir -p $(BUILD)/engine/entities
	@mkdir -p $(BUILD)/engine/core
	@mkdir -p $(BUILD)/engine/core/sprite
	@mkdir -p $(BUILD)/game
	$(CXX) $(CXXFLAGS) $(RAYLIB_INC) -c $< -o $@

check-platform:
	@sh -c '\
	if [ "$(UNAME_S)" != "Linux" ] || [ "$(UNAME_M)" != "x86_64" ]; then \
		echo "Error: This Makefile only supports Linux x86_64 (amd64)"; \
		echo "Detected: $(UNAME_S) $(UNAME_M)"; \
		exit 1; \
	fi; \
	if [ ! -d "$(RAYLIB_FOLDER)" ]; then \
		echo "Downloading raylib 5.5 for Linux amd64..."; \
		wget https://github.com/raysan5/raylib/releases/download/5.5/raylib-5.5_linux_amd64.tar.gz; \
		tar -xzf raylib-5.5_linux_amd64.tar.gz; \
		rm raylib-5.5_linux_amd64.tar.gz; \
		echo "raylib downloaded and extracted"; \
	fi'

clean:
	rm -rf $(BUILD)
	rm -rf $(RAYLIB_FOLDER)

