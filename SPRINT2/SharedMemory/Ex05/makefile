#makefile

main: main.o
		gcc -Wall -g main.o -o main -lrt

main.o: main.c
		gcc -g -Wall -c main.c -o main.o -lrt

clean:
		rm -f *.o main

run: main
		./main
