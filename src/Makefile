CC = g++
CFLAGS = -std=c++11

OBJ_FILES = io.o main.o system.o

all: main

main: $(OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $@

io.o: io.cpp io.hpp
	$(CC) $(CFLAGS) -c $< -o $@

main.o: main.cpp io.hpp system.hpp
	$(CC) $(CFLAGS) -c $< -o $@

system.o: system.cpp system.hpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_FILES)
