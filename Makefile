CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 $(shell sdl2-config --cflags)
LDFLAGS = $(shell sdl2-config --libs)
TARGET  = chip8
SRCS    = chip8.c decode.c

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: clean
