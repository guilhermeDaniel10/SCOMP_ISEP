#makefile
ex06: main.o
		gcc main.o -o ex06 -lrt -lpthread

main.o: main.c
		gcc -g -Wall -c main.c -o main.o -lrt -lpthread

clean:
		rm -f *.o ex06

run: ex06
		./ex06
