SRC = src
BUILD = build
RAYLIB_FOLDER = raylib-5.5_linux_amd64

CS = \
		 $(SRC)/main.c

OBJ = \
			$(BUILD)/main.o

CC = gcc
CFLAGS = -Wall -Wextra -g

# Detect platform
UNAME_S := $(shell uname -s)
UNAME_M := $(shell uname -m)

.PHONY: all clean check-platform

all: check-platform $(BUILD)/vsc

$(BUILD)/vsc: $(OBJ)
	$(CC) -o $@ $^ -L$(RAYLIB_FOLDER)/lib -l:libraylib.a -lm

$(BUILD)/%.o: $(SRC)/%.c 
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@ -I$(RAYLIB_FOLDER)/include

check-platform:
	@if [ "$(UNAME_S)" != "Linux" ] || [ "$(UNAME_M)" != "x86_64" ]; then \
		echo "Error: This Makefile only supports Linux x86_64 (amd64)"; \
		echo "Detected: $(UNAME_S) $(UNAME_M)"; \
		exit 1; \
	fi
	@if [ ! -d "$(RAYLIB_FOLDER)" ]; then \
		echo "Downloading raylib 5.5 for Linux amd64..."; \
		wget https://github.com/raysan5/raylib/releases/download/5.5/raylib-5.5_linux_amd64.tar.gz; \
		tar -xzf raylib-5.5_linux_amd64.tar.gz; \
		rm raylib-5.5_linux_amd64.tar.gz; \
		echo "raylib downloaded and extracted"; \
	fi

clean:
	rm -rf $(BUILD)
	rm -rf raylib-5.5_linux_amd64
