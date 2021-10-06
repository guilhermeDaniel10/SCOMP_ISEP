#makefile
ex11: main.o
		gcc main.o -o ex11

main.o: main.c
		gcc -g -Wall -c main.c -o main.o

clean:
		rm -f *.o ex09

run: ex11
		./ex11
