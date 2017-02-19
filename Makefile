CC=gcc
CFLAGS=-c -Wall -I $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
LDFLAGS=
EXECUTABLE=media-manager
SOURCES=media-manager.c
OBJECTS=$(SOURCES:%.c=%.o)

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
