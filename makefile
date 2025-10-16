# Compiler and flags
CC = gcc
CFLAGS = -Wall -std=c11

# Use pkg-config to get libssh flags
PKG_CFLAGS = $(shell pkg-config --cflags libssh)
PKG_LIBS = $(shell pkg-config --libs libssh)

# Sources and headers
SRC = main.c autorizzaci.c
HDR = main.h autorizzaci.h

# Output binary
TARGET = SSHPotica

# Default target
all: $(TARGET)

$(TARGET): $(SRC) $(HDR)
	$(CC) $(CFLAGS) $(PKG_CFLAGS) -o $(TARGET) $(SRC) $(PKG_LIBS)

clean:
	rm -f $(TARGET)
