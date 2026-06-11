# Compiler
CC = gcc
PYTHON = python3

# Flags
CFLAGS = -Wall -Wextra -O2
LDFLAGS = -lm

# Directories
SRC_DIR = .
OBJ_DIR = build
BIN_DIR = bin
OUT_DIR = outputs
VENV_DIR = .venv
VENV_PYTHON = $(VENV_DIR)/bin/python
VENV_PIP = $(VENV_DIR)/bin/pip
VENV_STAMP = $(VENV_DIR)/.deps-installed

# Files
SRC = $(SRC_DIR)/random_number_generation.c
OBJ = $(OBJ_DIR)/random_number_generation.o
TARGET = $(BIN_DIR)/random_number_generation

# Default target
all: $(TARGET)

# Link
$(TARGET): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Compile
$(OBJ): $(SRC)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $(SRC) -o $(OBJ)

# Run program
run: $(TARGET)
	./$(TARGET)

# Create a local Python virtual environment for plotting dependencies
$(VENV_STAMP):
	$(PYTHON) -m venv $(VENV_DIR)
	$(VENV_PIP) install --upgrade pip
	$(VENV_PIP) install numpy matplotlib
	@touch $(VENV_STAMP)

venv: $(VENV_STAMP)

# Run python plotting
plot-python: run $(VENV_STAMP)
	$(VENV_PYTHON) plots.py

# Clean
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(OUT_DIR)

clean-venv:
	rm -rf $(VENV_DIR)

# Phony targets
.PHONY: all run venv plot-python clean clean-venv
