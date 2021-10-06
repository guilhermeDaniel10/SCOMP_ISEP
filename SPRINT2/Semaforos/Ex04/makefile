#makefile
ex04: main.o
		gcc main.o -o ex04 -lrt -lpthread

main.o: main.c
		gcc -g -Wall -c main.c -o main.o -lrt -lpthread

clean:
		rm -f *.o ex04

run: ex04
		./ex04
