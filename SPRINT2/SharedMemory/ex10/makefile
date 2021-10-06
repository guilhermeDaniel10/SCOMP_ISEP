#makefile
ex10: main.o
		gcc -Wall -g main.o -o ex10 -lrt

main.o: main.c
		gcc -g -Wall -c main.c -o main.o -lrt

clean:
		rm -f *.o ex10

run: ex10
		./ex10
