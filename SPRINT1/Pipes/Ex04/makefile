#makefile
ex04: main.o
		gcc main.o -o ex04

main.o: main.c
		gcc -g -Wall -c main.c -o main.o

clean:
		rm -f *.o ex04

run: ex04
		./ex04
