#makefile
ex05: main.o
		gcc main.o -o ex05 -lrt -lpthread

main.o: main.c
		gcc -g -Wall -c main.c -o main.o -lrt -lpthread

clean:
		rm -f *.o ex05

run: ex05
		./ex05
