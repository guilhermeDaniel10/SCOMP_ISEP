#makefile
ex11: main.o
		gcc main.o -o ex11 -lrt -lpthread

main.o: main.c
		gcc -g -Wall -c main.c -o main.o -lrt -lpthread

clean:
		rm -f *.o ex11

run: ex11
		./ex11
