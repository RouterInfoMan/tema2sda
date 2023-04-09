CFLAGS = -g -Wall

all: main.o imagefuncs.o
	gcc -o quadtree main.o imagefuncs.o $(CFLAGS)

main.o: main.c
	gcc -c main.c $(CFLAGS)

imagefuncs.o: imagefuncs.c
	gcc -c imagefuncs.c $(CFLAGS)

clean:
	rm *.o quadtree