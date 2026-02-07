SRC = src
BUILD = build
RAYLIB_FOLDER = raylib-5.5_linux_amd64
LIB_DIR = lib
ONLINE_EXAMPLE = example/online_game
OFFLINE_EXAMPLE = example/solo_game
STATIC_LIB = $(LIB_DIR)/libshabby.a

SHABBY_LIB_SRC = \
	$(SRC)/entities/entity.cpp \
	$(SRC)/entities/entity_manager/entity_manager.cpp \
	$(SRC)/entities/controllers/entity_controller.cpp \
	$(SRC)/entities/controllers/networked_entity_controller.cpp \
	$(SRC)/replication/snapshot/snapshot.cpp \
	$(SRC)/replication/snapshot/entity_snapshot.cpp \
	$(SRC)/replication/snapshot/world_snapshot.cpp \
	$(SRC)/core/engine/engine.cpp \
	$(SRC)/core/sprite/sprite.cpp \
	$(SRC)/core/sprite/animated_sprite.cpp \
	$(SRC)/core/render/render_system.cpp \
	$(SRC)/core/game_loop/game_loop.cpp \
	$(SRC)/scene/scene.cpp \
	$(SRC)/networking/server.cpp \
	$(SRC)/networking/client.cpp \
	$(SRC)/networking/protocol/network_packet.cpp \
	$(SRC)/networking/protocol/packet_registry.cpp \
	$(SRC)/networking/handlers/entity_spawn_handler.cpp \
	$(SRC)/networking/handlers/entity_destroy_handler.cpp \
	$(SRC)/networking/handlers/snapshot_handler.cpp \
	$(SRC)/networking/handlers/input_command_handler.cpp \
	$(SRC)/networking/packet_handler.cpp \
	$(SRC)/core/scheduler/scheduler.cpp

EXAMPLE_SRC = \
	$(ONLINE_EXAMPLE)/main.cpp \
	$(ONLINE_EXAMPLE)/actors/player_controller.cpp \
	$(ONLINE_EXAMPLE)/server/logic/example_logic.cpp

EXAMPLE_OFFLINE_SRC = \
	$(OFFLINE_EXAMPLE)/main.cpp \
	$(OFFLINE_EXAMPLE)/actors/player_controller.cpp

SHABBY_LIB_OBJ = \
	$(BUILD)/entities/entity.o \
	$(BUILD)/entities/entity_manager/entity_manager.o \
	$(BUILD)/entities/controllers/entity_controller.o \
	$(BUILD)/entities/controllers/networked_entity_controller.o \
	$(BUILD)/replication/snapshot/snapshot.o \
	$(BUILD)/replication/snapshot/entity_snapshot.o \
	$(BUILD)/replication/snapshot/world_snapshot.o \
	$(BUILD)/core/engine/engine.o \
	$(BUILD)/core/sprite/sprite.o \
	$(BUILD)/core/sprite/animated_sprite.o \
	$(BUILD)/core/render/render_system.o \
	$(BUILD)/core/game_loop/game_loop.o \
	$(BUILD)/scene/scene.o \
	$(BUILD)/networking/server.o \
	$(BUILD)/networking/client.o \
	$(BUILD)/networking/protocol/network_packet.o \
	$(BUILD)/networking/protocol/packet_registry.o \
	$(BUILD)/networking/handlers/entity_spawn_handler.o \
	$(BUILD)/networking/handlers/entity_destroy_handler.o \
	$(BUILD)/networking/handlers/snapshot_handler.o \
	$(BUILD)/networking/handlers/input_command_handler.o \
	$(BUILD)/networking/packet_handler.o \
	$(BUILD)/core/scheduler/scheduler.o


EXAMPLE_OBJ = \
	$(BUILD)/online_game/main.o \
	$(BUILD)/online_game/actors/player_controller.o \
	$(BUILD)/online_game/server/logic/example_logic.o

EXAMPLE_OFFLINE_OBJ = \
	$(BUILD)/solo_game/main.o \
	$(BUILD)/solo_game/actors/player_controller.o

CXX = g++

CXXFLAGS = -std=c++20 -Wall -Wextra -g -Iinclude

RAYLIB_INC = -isystem $(RAYLIB_FOLDER)/include
RAYLIB_LIB = -L$(RAYLIB_FOLDER)/lib -l:libraylib.a -lm
SHABBY_LIB = -L$(LIB_DIR)

UNAME_S := $(shell uname -s)
UNAME_M := $(shell uname -m)

.PHONY: all clean check-platform clean-src lib clean-lib clean-raylib solo-example

all: check-platform $(STATIC_LIB) $(BUILD)/online_game_exe

lib: check-platform $(STATIC_LIB)

solo-example: check-platform $(STATIC_LIB) $(BUILD)/solo_game_exe

$(STATIC_LIB): $(SHABBY_LIB_OBJ)
	@mkdir -p $(LIB_DIR)
	ar -rcs $@ $^

$(BUILD)/%.o: $(SRC)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(RAYLIB_INC) -c $< -o $@

$(BUILD)/online_game/%.o: $(ONLINE_EXAMPLE)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -Iexample/online_game $(RAYLIB_INC) -c $< -o $@

$(BUILD)/online_game_exe: $(EXAMPLE_OBJ) $(STATIC_LIB)
	@mkdir -p $(dir $@)
	$(CXX) -o $@ $(EXAMPLE_OBJ) -L$(LIB_DIR) -lshabby $(RAYLIB_LIB)

$(BUILD)/solo_game/%.o: $(OFFLINE_EXAMPLE)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -Iexample/solo_game $(RAYLIB_INC) -c $< -o $@

$(BUILD)/solo_game_exe: $(EXAMPLE_OFFLINE_OBJ) $(STATIC_LIB)
	@mkdir -p $(dir $@)
	$(CXX) -o $@ $(EXAMPLE_OFFLINE_OBJ) -L$(LIB_DIR) -lshabby $(RAYLIB_LIB)

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
