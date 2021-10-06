#makefile
ex02: main.o
		gcc main.o -o ex02

main.o: main.c
		gcc -g -Wall -c main.c -o main.o

clean:
		rm -f *.o ex02

run: ex02
		./ex02
