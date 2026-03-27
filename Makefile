SRC_DIR     := src
BUILD_DIR   := build
INCLUDE_DIR := include
LIB_DIR     := lib

ONLINE_EXAMPLE_DIR  := example/online_game
OFFLINE_EXAMPLE_DIR := example/offline_game

ONLINE_EXAMPLE_BUILD_DIR  := build/online_game
OFFLINE_EXAMPLE_BUILD_DIR := build/offline_game

ONLINE_EXAMPLE_TARGET  := $(BUILD_DIR)/online_example
OFFLINE_EXAMPLE_TARGET := $(BUILD_DIR)/offline_example
STATIC_LIB_TARGET      := $(LIB_DIR)/libshabby.a

SHABBY_LIB_SRC = \
  $(SRC_DIR)/entities/entity.cpp \
  $(SRC_DIR)/entities/entity_manager/entity_manager.cpp \
  $(SRC_DIR)/entities/controllers/entity_controller.cpp \
  $(SRC_DIR)/entities/controllers/networked_entity_controller.cpp \
  $(SRC_DIR)/replication/snapshot/snapshot.cpp \
  $(SRC_DIR)/replication/snapshot/entity_snapshot.cpp \
  $(SRC_DIR)/replication/snapshot/world_snapshot.cpp \
  $(SRC_DIR)/core/engine/engine.cpp \
  $(SRC_DIR)/core/sprite/sprite.cpp \
  $(SRC_DIR)/core/sprite/animated_sprite.cpp \
  $(SRC_DIR)/core/render/render_system.cpp \
  $(SRC_DIR)/core/game_loop/game_loop.cpp \
  $(SRC_DIR)/scene/scene.cpp \
  $(SRC_DIR)/networking/server.cpp \
  $(SRC_DIR)/networking/client.cpp \
  $(SRC_DIR)/networking/protocol/network_packet.cpp \
  $(SRC_DIR)/networking/protocol/packet_registry.cpp \
  $(SRC_DIR)/networking/handlers/entity_spawn_handler.cpp \
  $(SRC_DIR)/networking/handlers/entity_destroy_handler.cpp \
  $(SRC_DIR)/networking/handlers/snapshot_handler.cpp \
  $(SRC_DIR)/networking/handlers/input_command_handler.cpp \
  $(SRC_DIR)/networking/packet_handler.cpp \
  $(SRC_DIR)/core/scheduler/scheduler.cpp \
  $(SRC_DIR)/physics/hitbox/rectangle_hitbox.cpp \
  $(SRC_DIR)/physics/hitbox/hitbox.cpp \
  $(SRC_DIR)/physics/collision/collision_system.cpp \
  $(SRC_DIR)/networking/handlers/entity_spawn_with_hitbox_handler.cpp

EXAMPLE_ONLINE_SRC = \
  $(ONLINE_EXAMPLE_DIR)/main.cpp \
  $(ONLINE_EXAMPLE_DIR)/actors/player_controller.cpp \
  $(ONLINE_EXAMPLE_DIR)/actors/player_controller_base.cpp \
  $(ONLINE_EXAMPLE_DIR)/server/logic/example_logic.cpp

EXAMPLE_OFFLINE_SRC = \
  $(OFFLINE_EXAMPLE_DIR)/main.cpp \
  $(OFFLINE_EXAMPLE_DIR)/actors/player_controller.cpp \
  $(OFFLINE_EXAMPLE_DIR)/actors/ennemy_controller.cpp

SHABBY_LIB_OBJ = \
  $(BUILD_DIR)/entities/entity.o \
  $(BUILD_DIR)/entities/entity_manager/entity_manager.o \
  $(BUILD_DIR)/entities/controllers/entity_controller.o \
  $(BUILD_DIR)/entities/controllers/networked_entity_controller.o \
  $(BUILD_DIR)/replication/snapshot/snapshot.o \
  $(BUILD_DIR)/replication/snapshot/entity_snapshot.o \
  $(BUILD_DIR)/replication/snapshot/world_snapshot.o \
  $(BUILD_DIR)/core/engine/engine.o \
  $(BUILD_DIR)/core/sprite/sprite.o \
  $(BUILD_DIR)/core/sprite/animated_sprite.o \
  $(BUILD_DIR)/core/render/render_system.o \
  $(BUILD_DIR)/core/game_loop/game_loop.o \
  $(BUILD_DIR)/scene/scene.o \
  $(BUILD_DIR)/networking/server.o \
  $(BUILD_DIR)/networking/client.o \
  $(BUILD_DIR)/networking/protocol/network_packet.o \
  $(BUILD_DIR)/networking/protocol/packet_registry.o \
  $(BUILD_DIR)/networking/handlers/entity_spawn_handler.o \
  $(BUILD_DIR)/networking/handlers/entity_destroy_handler.o \
  $(BUILD_DIR)/networking/handlers/snapshot_handler.o \
  $(BUILD_DIR)/networking/handlers/input_command_handler.o \
  $(BUILD_DIR)/networking/packet_handler.o \
  $(BUILD_DIR)/core/scheduler/scheduler.o \
  $(BUILD_DIR)/physics/hitbox/rectangle_hitbox.o \
  $(BUILD_DIR)/physics/hitbox/hitbox.o \
  $(BUILD_DIR)/physics/collision/collision_system.o \
  $(BUILD_DIR)/networking/handlers/entity_spawn_with_hitbox_handler.o

ONLINE_EXAMPLE_OBJ = \
  $(ONLINE_EXAMPLE_BUILD_DIR)/main.o \
  $(ONLINE_EXAMPLE_BUILD_DIR)/actors/player_controller.o \
  $(ONLINE_EXAMPLE_BUILD_DIR)/actors/player_controller_base.o \
  $(ONLINE_EXAMPLE_BUILD_DIR)/server/logic/example_logic.o

OFFLINE_EXAMPLE_OBJ = \
  $(OFFLINE_EXAMPLE_BUILD_DIR)/main.o \
  $(OFFLINE_EXAMPLE_BUILD_DIR)/actors/player_controller.o \
  $(OFFLINE_EXAMPLE_BUILD_DIR)/actors/ennemy_controller.o

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

.PHONY: all clean clean-complete check-platform clean-raylib examples

all: check-raylib $(STATIC_LIB_TARGET)

$(OFFLINE_EXAMPLE_BUILD_DIR)/%.o: $(OFFLINE_EXAMPLE_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I$(OFFLINE_EXAMPLE_DIR) $(RAYLIB_INC) -c $< -o $@

$(ONLINE_EXAMPLE_BUILD_DIR)/%.o: $(ONLINE_EXAMPLE_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I$(ONLINE_EXAMPLE_DIR) $(RAYLIB_INC) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(RAYLIB_INC) -c $< -o $@

$(STATIC_LIB_TARGET): $(SHABBY_LIB_OBJ)
	@mkdir -p $(LIB_DIR)
	ar -rcs $@ $^

examples: $(ONLINE_EXAMPLE_TARGET) $(OFFLINE_EXAMPLE_TARGET)

$(OFFLINE_EXAMPLE_TARGET): $(OFFLINE_EXAMPLE_OBJ) $(STATIC_LIB_TARGET)
	@mkdir -p $(dir $@)
	$(CXX) -o $@ $(OFFLINE_EXAMPLE_OBJ) -L$(LIB_DIR) -lshabby $(LDFLAGS)

$(ONLINE_EXAMPLE_TARGET): $(ONLINE_EXAMPLE_OBJ) $(STATIC_LIB_TARGET)
	@mkdir -p $(dir $@)
	$(CXX) -o $@ $(ONLINE_EXAMPLE_OBJ) -L$(LIB_DIR) -lshabby $(LDFLAGS)

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
