#makefile
ex08: main.o
		gcc main.o -o ex08

main.o: main.c
		gcc -g -Wall -c main.c -o main.o

clean:
		rm -f *.o ex08

run: ex08
		./ex08
