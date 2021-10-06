#makefile
ex15: main.o
		gcc main.o -o ex15 -lrt -lpthread

main.o: main.c
		gcc -g -Wall -c main.c -o main.o -lrt -lpthread

clean:
		rm -f *.o ex15

run: ex15
		./ex15
