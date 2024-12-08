CC = gcc
CFLAGS = -Wall -Wextra -O2 -lm

SRC = main.c
OBJ = main.o
EXE = c_color

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(OBJ) -o $(EXE) $(CFLAGS)

main.o: main.c stb_image.h
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f $(OBJ) $(EXE)

install: $(EXE)
	cp $(EXE) /usr/local/bin/

help:
	@echo "Makefile for c_color program"
	@echo "Usage:"
	@echo "  make         Build the c_color executable"
	@echo "  make clean   Remove compiled object files and executable"
	@echo "  make install Install the program"
	@echo "  make help    Show this message"
