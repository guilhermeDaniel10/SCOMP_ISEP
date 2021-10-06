#makefile
ex08: main.o
		gcc main.o -o ex08 -lrt

main.o: main.c
		gcc -g -Wall -c main.c -o main.o -lrt

clean:
		rm -f *.o ex08

run: ex08
		./ex08
