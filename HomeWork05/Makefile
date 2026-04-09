# Compiler
CC = gcc
PYTHON = python3

# Flags
CFLAGS = -Wall -Wextra -O2 -Iinclude
LDFLAGS = -lm

# Directories
SRC_DIR = src
OBJ_DIR = build
BIN_DIR = bin
OUT_DIR = outputs
VENV_DIR = .venv
VENV_PYTHON = $(VENV_DIR)/bin/python
VENV_PIP = $(VENV_DIR)/bin/pip
VENV_STAMP = $(VENV_DIR)/.deps-installed

# Files
SRC = $(SRC_DIR)/compute.c $(SRC_DIR)/legendre_polynomials.c $(SRC_DIR)/spherical_harm.c
OBJ = $(OBJ_DIR)/compute.o $(OBJ_DIR)/legendre_polynomials.o $(OBJ_DIR)/spherical_harm.o
TARGET = $(BIN_DIR)/compute

# Default target
all: $(TARGET)

# Link
$(TARGET): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Compile
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

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
	$(VENV_PYTHON) scripts/plot_errors.py

# Clean
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(OUT_DIR) *.csv

clean-venv:
	rm -rf $(VENV_DIR)

# Phony targets
.PHONY: all run venv plot-python clean clean-venv