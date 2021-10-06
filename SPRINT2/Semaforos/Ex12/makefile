#makefile
main:
	gcc -Wall main.c -o main.o -lrt -lpthread
	gcc -Wall clients.c -o clients.o -lrt -lpthread
	gcc -Wall salesman.c -o salesman.o -lrt -lpthread

main.o: main.c
	gcc -Wall -g -c main.c -o main.o -lrt -lpthread

run:
	gcc -Wall main.c -o main.o -lrt -lpthread
	gcc -Wall clients.c -o clients.o -lrt -lpthread
	gcc -Wall salesman.c -o salesman.o -lrt -lpthread
	./main.o

clean:
	rm *.o
	rm /dev/shm/*
