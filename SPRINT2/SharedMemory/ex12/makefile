#makefile
ex12: main.o
		gcc main.o -o ex12 -lrt

main.o: main.c
		gcc -g -Wall -c main.c -o main.o -lrt

clean:
		rm -f *.o ex12

run: ex12
		./ex12
