#makefile
ex09: main.o
		gcc main.o -o ex09

main.o: main.c
		gcc -g -Wall -c main.c -o main.o

clean:
		rm -f *.o ex09

run: ex09
		./ex09
