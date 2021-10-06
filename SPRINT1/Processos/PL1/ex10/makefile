#makefile
ex10: main.o
		gcc main.o -o ex10

main.o: main.c
		gcc -g -Wall -c main.c -o main.o

clean:
		rm -f *.o ex10

run: ex10
		./ex10
