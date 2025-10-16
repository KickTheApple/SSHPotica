CC = gcc
CFLAGS = -Wall -std=c11

PKG_CFLAGS = $(shell pkg-config --cflags libssh)
PKG_LIBS = $(shell pkg-config --libs libssh)

SRC = main.c autorizzaci.c
HDR = main.h autorizzaci.h

TARGET = SSHPotica

all: $(TARGET)

$(TARGET): $(SRC) $(HDR)
	$(CC) $(CFLAGS) $(PKG_CFLAGS) -o $(TARGET) $(SRC) $(PKG_LIBS)

clean:
	rm -f $(TARGET)
