#makefile
ex17: main.o
		gcc main.o -o ex17 -lrt -lpthread

main.o: main.c
		gcc -g -Wall -c main.c -o main.o -lrt -lpthread

clean:
		rm -f *.o ex17

run: ex17
		./ex17
