#makefile
ex01: main.o
		gcc main.o -o ex01 -lrt -lpthread

main.o: main.c
		gcc -g -Wall -c main.c -o main.o -lrt -lpthread

clean:
		rm -f *.o ex01

run: ex01
		./ex01
