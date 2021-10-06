#makefile
ex03: main.o
		gcc main.o -o ex03

main.o: main.c
		gcc -g -Wall -c main.c -o main.o

clean:
		rm -f *.o ex03

run: ex03
		./ex03
