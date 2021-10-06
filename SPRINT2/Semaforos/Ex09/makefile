#makefile
ex09: main.o
		gcc main.o -o ex09 -lrt -lpthread

main.o: main.c
		gcc -g -Wall -c main.c -o main.o -lrt -lpthread

clean:
		rm -f *.o ex09

run: ex09
		./ex09
