#makefile
ex07: main.o
		gcc main.o -o ex07

main.o: main.c
		gcc -g -Wall -c main.c -o main.o

clean:
		rm -f *.o ex07

run: ex07
		./ex07
