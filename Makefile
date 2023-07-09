CC = g++
CFLAGS = -Wall -Wextra
LDFLAGS = -lz

all: start

start: start.cpp
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

run: start
	./start

.PHONY: clean
clean:
	rm -f start
