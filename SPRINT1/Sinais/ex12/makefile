#makefile
ex12: main.o
		gcc main.o -o ex12

main.o: main.c
		gcc -g -Wall -c main.c -o main.o

clean:
		rm -f *.o ex12

run: ex12
		./ex12
