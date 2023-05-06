CFLAGS = -g -Wall

build: main.o imagefuncs.o
	gcc -o quadtree main.o imagefuncs.o $(CFLAGS)

main.o: main.c
	gcc -c main.c $(CFLAGS)

imagefuncs.o: imagefuncs.c
	gcc -c imagefuncs.c $(CFLAGS)

zip: imagefuncs.c main.c imagefuncs.h README Makefile README
	zip tema2sda.zip imagefuncs.c main.c imagefuncs.h README Makefile README

clean:
	rm *.o quadtree