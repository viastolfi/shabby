SRC = src
BUILD = build
RAYLIB_FOLDER = raylib-5.5_linux_amd64
LIB_DIR = lib
EXAMPLE = example
STATIC_LIB = $(LIB_DIR)/libshabby.a

SHABBY_LIB_SRC = \
	$(SRC)/entities/entities.cpp \
	$(SRC)/entities/entity_manager.cpp \
	$(SRC)/replication/snapshot/snapshot.cpp \
	$(SRC)/replication/snapshot/entity_snapshot.cpp \
	$(SRC)/core/engine/engine.cpp \
	$(SRC)/core/sprite/sprite.cpp \
	$(SRC)/core/sprite/animated_sprite.cpp \
	$(SRC)/scene/scene.cpp \
	$(SRC)/networking/server.cpp \
	$(SRC)/networking/network_manager.cpp 

EXAMPLE_SRC = \
	$(EXAMPLE)/main.cpp \
	$(EXAMPLE)/game/player.cpp \
	$(EXAMPLE)/game/main_scene.cpp \
	$(EXAMPLE)/game/ennemy.cpp

SHABBY_LIB_OBJ = \
	$(BUILD)/entities/entities.o \
	$(BUILD)/entities/entity_manager.o \
	$(BUILD)/replication/snapshot/snapshot.o \
	$(BUILD)/replication/snapshot/entity_snapshot.o \
	$(BUILD)/core/engine/engine.o \
	$(BUILD)/core/sprite/sprite.o \
	$(BUILD)/core/sprite/animated_sprite.o \
	$(BUILD)/scene/scene.o \
	$(BUILD)/networking/server.o \
	$(BUILD)/networking/network_manager.o 

EXAMPLE_OBJ = \
	$(BUILD)/main.o \
	$(BUILD)/game/player.o \
	$(BUILD)/game/main_scene.o 

CXX = g++

CXXFLAGS = -std=c++17 -Wall -Wextra -g -Iinclude

RAYLIB_INC = -isystem $(RAYLIB_FOLDER)/include
RAYLIB_LIB = -L$(RAYLIB_FOLDER)/lib -l:libraylib.a -lm
SHABBY_LIB = -L$(LIB_DIR)

UNAME_S := $(shell uname -s)
UNAME_M := $(shell uname -m)

.PHONY: all clean check-platform clean-src lib clean-lib clean-raylib

all: check-platform $(STATIC_LIB) $(BUILD)/example_game

lib: check-platform $(STATIC_LIB)

$(STATIC_LIB): $(SHABBY_LIB_OBJ)
	@mkdir -p $(LIB_DIR)
	ar -rcs $@ $^

$(BUILD)/%.o: $(SRC)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(RAYLIB_INC) -c $< -o $@

$(BUILD)/%.o: $(EXAMPLE)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(RAYLIB_INC) -c $< -o $@

$(BUILD)/example_game: $(EXAMPLE_OBJ) $(STATIC_LIB)
	$(CXX) -o $@ $(EXAMPLE_OBJ) -L$(LIB_DIR) -lshabby $(RAYLIB_LIB)

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

clean: clean-src clean-lib

clean-raylib:
	rm -rf $(RAYLIB_FOLDER)

clean-src:
	rm -rf $(BUILD)

clean-lib:
	rm -rf $(LIB_DIR)
