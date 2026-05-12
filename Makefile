# Compile and link settings
CC = gcc
CFLAGS = -Wall -O2 -fopenmp -I../include
LDFLAGS = -fopenmp -lm

# --- DIRECTORIES ---
SRC_DIR    := src
OBJ_DIR    := build
BIN_DIR    := bin

# --- OBJECTS ---
SORTING_OBJ   := $(OBJ_DIR)/sorting.o
PARAMS_OBJ := $(OBJ_DIR)/params.o

# --- TARGETS ---
TARGET := $(BIN_DIR)/sorting

# --- RULES ---
# Default [all targets]
all: $(TARGET)
	@echo "All targets compiled. Me go home."


# Linking Sequential
$(TARGET): $(SORTING_OBJ) $(PARAMS_OBJ) | $(BIN_DIR)
	$(CC) $^ $(LDFLAGS) -o $@
	@echo "Compilation of $@ completed."

# Compilation generic rule
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Folders
$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

# Cleaning
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "Cleaning done. Me go home."

# Treat as execution cmds
.PHONY: all clean