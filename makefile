# Compiler to use
CC = gcc

# Target executable name
TARGET = program2

# Source file
SRC = program_2.c

# Default rule
all: $(TARGET)

# Rule to create the executable
$(TARGET):
	$(CC) $(SRC) -o $(TARGET)

# Clean rule to remove the executable
clean:
	rm -f $(TARGET)

.PHONY: all clean
