all: towerofhanoi

towerofhanoi: towerofhanoi.o primlib.o
	gcc -g $^ -o $@  -lSDL2_gfx `sdl2-config --libs` -lm

.c.o: 
	gcc -g -Wall -pedantic `sdl2-config --cflags` -c  $<

primlib.o: primlib.c primlib.h

towerofhanoi.o: towerofhanoi.c primlib.h

clean:
	-rm primlib.o towerofhanoi.o towerofhanoi
	
