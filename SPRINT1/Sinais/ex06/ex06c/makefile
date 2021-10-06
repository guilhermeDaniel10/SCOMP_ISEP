#makefile
ex06: main.o
		gcc main.o -o ex06

main.o: main.c
		gcc -g -Wall -c main.c -o main.o

clean:
		rm -f *.o ex06

run: ex06
		./ex06
