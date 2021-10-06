#makefile
main:
	gcc -Wall main.c -o main.o -lrt -lpthread
	gcc -Wall consult.c -o consult.o -lrt -lpthread
	gcc -Wall insert.c -o insert.o -lrt -lpthread
	gcc -Wall consult_all.c -o consult_all.o -lrt -lpthread

main.o: main.c
	gcc -Wall -g -c main.c -o main.o -lrt -lpthread

run:
	gcc -Wall main.c -o main.o -lrt -lpthread
	gcc -Wall consult.c -o consult.o -lrt -lpthread
	gcc -Wall insert.c -o insert.o -lrt -lpthread
	gcc -Wall consult_all.c -o consult_all.o -lrt -lpthread
	./main.o

clean:
	rm *.o
	rm /dev/shm/*
