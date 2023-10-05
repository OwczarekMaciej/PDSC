all: main

main: main.o
	gcc -g $^ -o $@  -lSDL2_gfx `sdl2-config --libs` -lm

.c.o: 
	gcc -g -Wall -pedantic `sdl2-config --cflags` -c  $<


main.o: main.c

clean:
	-rm main.o main
