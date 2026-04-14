H5CC ?= /opt/homebrew/bin/h5cc
CFLAGS ?= -O2 -Wall -Wextra -Wpedantic -std=c11
LDLIBS ?= -lm
TARGET := sum2vec
CONFIG := config.txt

all: $(TARGET)

$(TARGET): sum2vec.c
	$(H5CC) $(CFLAGS) $< -o $@ $(LDLIBS)

run: $(TARGET)
	./$(TARGET) $(CONFIG)

clean:
	rm -f $(TARGET) sum2vec_output.h5

.PHONY: all run clean