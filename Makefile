CC = gcc
LIBS = -lncurses
FLAGS = -Wall -Wextra -pedantic -std=c99

snake: 
	$(CC) $(FLAGS) main.c snake.c game.c -o snake $(LIBS)

clean:
	rm snake