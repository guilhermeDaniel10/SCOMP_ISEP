#makefile
ex08: main.o
		gcc main.o -o ex08 -lrt -lpthread

main.o: main.c
		gcc -g -Wall -c main.c -o main.o -lrt -lpthread

clean:
		rm -f *.o ex08

run: ex08
		./ex08
