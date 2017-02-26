CC=gcc
CFLAGS=-c -Wall -I $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
LDFLAGS=-lforms
EXECUTABLE=media-manager
SOURCES=media-manager.c list/list.c medium/medium.c interface/cli/cli.c interface/libforms/libforms.c interface/libforms/gui.c shared/shared.c
OBJECTS=$(SOURCES:%.c=%.o)

.PHONY: clean

all: $(EXECUTABLE)

debug: CFLAGS += -DDEBUG -g
debug: LDFLAGS += -DDEBUG -g
debug: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS)
	rm -f $(EXECUTABLE)
