CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lSDL -lSDL_image -lSDL_ttf -lSDL_mixer
OBJ = main.o

all: main

main: $(OBJ)
	$(CC) $(OBJ) -o main $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o main

