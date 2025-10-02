# Simple Makefile for The Craft demo
CC = gcc
CFLAGS = -O2 -Wall `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs`

SRC = src/main.c src/world.c
OBJ = $(SRC:.c=.o)

all: thecraft

thecraft: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) thecraft

.PHONY: all clean
