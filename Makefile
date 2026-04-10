SRC_DIR     := src
BUILD_DIR   := build
INCLUDE_DIR := include
LIB_DIR     := lib

NODE_EXAMPLE_DIR        := example/node_example
ONLINE_NODE_EXAMPLE_DIR := example/online_node_example

NODE_EXAMPLE_BUILD_DIR        := build/node_game
ONLINE_NODE_EXAMPLE_BUILD_DIR := build/online_node_game

NODE_EXAMPLE_TARGET        := $(BUILD_DIR)/node_example
ONLINE_NODE_EXAMPLE_TARGET := $(BUILD_DIR)/online_node_example
STATIC_LIB_TARGET          := $(LIB_DIR)/libshabby.a

SHABBY_LIB_SRC = \
 	$(SRC_DIR)/node/inode.cpp \
  $(SRC_DIR)/node/sprite/sprite.cpp \
  $(SRC_DIR)/node/collision_shape/rigid_body.cpp \
	$(SRC_DIR)/core/engine/engine.cpp \
	$(SRC_DIR)/core/game_loop/game_loop.cpp \
	$(SRC_DIR)/core/render/render_system.cpp \
	$(SRC_DIR)/node/timer.cpp \
	$(SRC_DIR)/node/sprite/animated_sprite.cpp \
	$(SRC_DIR)/node/sprite/animation_player.cpp \
	$(SRC_DIR)/node/hitbox/rectangle_hitbox.cpp \
	$(SRC_DIR)/core/physics/collision_system.cpp \
	$(SRC_DIR)/node/scene/scene.cpp \
	$(SRC_DIR)/node/networking/server.cpp

NODE_EXAMPLE_SRC = \
	$(NODE_EXAMPLE_DIR)/main.cpp	 

ONLINE_NODE_EXAMPLE_SRC = \
	$(ONLINE_NODE_EXAMPLE_DIR)/main.cpp

SHABBY_LIB_OBJ = \
  $(BUILD_DIR)/node/inode.o \
  $(BUILD_DIR)/node/sprite/sprite.o \
  $(BUILD_DIR)/node/collision_shape/rigid_body.o \
	$(BUILD_DIR)/core/engine/engine.o \
	$(BUILD_DIR)/core/game_loop/game_loop.o \
	$(BUILD_DIR)/core/render/render_system.o \
	$(BUILD_DIR)/node/timer.o \
	$(BUILD_DIR)/node/sprite/animated_sprite.o \
	$(BUILD_DIR)/node/sprite/animation_player.o \
	$(BUILD_DIR)/node/hitbox/rectangle_hitbox.o \
	$(BUILD_DIR)/core/physics/collision_system.o \
	$(BUILD_DIR)/node/scene/scene.o \
	$(BUILD_DIR)/node/networking/server.o

NODE_EXAMPLE_OBJ = \
	$(NODE_EXAMPLE_BUILD_DIR)/main.o

ONLINE_NODE_EXAMPLE_OBJ = \
	$(ONLINE_NODE_EXAMPLE_BUILD_DIR)/main.o

UNAME_S := $(shell uname -s)
UNAME_M := $(shell uname -m)

ifeq ($(UNAME_S),Darwin)
    PLATFORM := macos
    RAYLIB_ARCHIVE := raylib-5.5_macos.tar.gz
    RAYLIB_DIR := raylib-5.5_macos

    LDFLAGS := ./$(RAYLIB_DIR)/lib/libraylib.a \
               -framework CoreVideo \
               -framework IOKit \
               -framework Cocoa \
               -framework GLUT \
               -framework OpenGL \
               -framework AudioToolbox

else ifeq ($(UNAME_S),Linux)
    ifeq ($(UNAME_M),x86_64)
        PLATFORM := linux
        RAYLIB_ARCHIVE := raylib-5.5_linux_amd64.tar.gz
        RAYLIB_DIR := raylib-5.5_linux_amd64

        LDFLAGS := -L$(RAYLIB_DIR)/lib -l:libraylib.a -lm
    else
        $(error Unsupported architecture: $(UNAME_M))
    endif
else
    $(error Unsupported OS: $(UNAME_S))
endif

CXX := g++
CXXFLAGS := -Wall -Wextra -g -I$(INCLUDE_DIR) -I$(RAYLIB_DIR)/include
RAYLIB_INC := -isystem $(RAYLIB_DIR)/include

.PHONY: all clean clean-complete check-platform clean-raylib examples install-dependencies

all: check-raylib $(STATIC_LIB_TARGET)

installed-dependencies: check-raylib

$(NODE_EXAMPLE_BUILD_DIR)/%.o: $(NODE_EXAMPLE_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I$(NODE_EXAMPLE_DIR) $(RAYLIB_INC) -c $< -o $@

$(ONLINE_NODE_EXAMPLE_BUILD_DIR)/%.o: $(ONLINE_NODE_EXAMPLE_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I$(ONLINE_NODE_EXAMPLE_DIR) $(RAYLIB_INC) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(RAYLIB_INC) -c $< -o $@

$(STATIC_LIB_TARGET): $(SHABBY_LIB_OBJ)
	@mkdir -p $(LIB_DIR)
	ar -rcs $@ $^

examples: $(NODE_EXAMPLE_TARGET) $(ONLINE_NODE_EXAMPLE_TARGET)

$(NODE_EXAMPLE_TARGET): $(NODE_EXAMPLE_OBJ) $(STATIC_LIB_TARGET)
	@mkdir -p $(dir $@)
	$(CXX) -o $@ $(NODE_EXAMPLE_OBJ) -L$(LIB_DIR) -lshabby $(LDFLAGS)

$(ONLINE_NODE_EXAMPLE_TARGET): $(ONLINE_NODE_EXAMPLE_OBJ) $(STATIC_LIB_TARGET)
	@mkdir -p $(dir $@)
	$(CXX) -o $@ $(ONLINE_NODE_EXAMPLE_OBJ) -L$(LIB_DIR) -lshabby $(LDFLAGS)

check-raylib:
	@if [ -d "$(RAYLIB_DIR)" ]; then \
		echo "raylib already installed"; \
	else \
		echo "Installing raylib for $(PLATFORM)..."; \
		mkdir $(RAYLIB_DIR); \
		wget https://github.com/raysan5/raylib/releases/download/5.5/$(RAYLIB_ARCHIVE); \
		tar -xzf $(RAYLIB_ARCHIVE) -C $(RAYLIB_DIR) --strip-components=1; \
		rm $(RAYLIB_ARCHIVE); \
		echo "raylib installed"; \
	fi

clean:
	rm -rf $(BUILD_DIR)

clean-complete: clean
	rm -rf $(LIB_DIR)
	rm -rf $(RAYLIB_DIR)
