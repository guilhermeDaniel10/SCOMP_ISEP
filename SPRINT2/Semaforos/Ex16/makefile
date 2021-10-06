#makefile
main:
	gcc -Wall main.c -o main.o -lrt -lpthread
	gcc -Wall East.c -o East.o -lrt -lpthread
	gcc -Wall West.c -o West.o -lrt -lpthread

main.o: main.c
	gcc -Wall -g -c main.c -o main.o -lrt -lpthread
