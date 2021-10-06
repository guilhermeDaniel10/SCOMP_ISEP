#makefile
initializer:
	gcc -Wall initializer.c -o initializer.o -lrt -lpthread
	gcc -Wall read.c -o read.o -lrt -lpthread
	gcc -Wall writter.c -o writter.o -lrt -lpthread
	gcc -Wall finisher.c -o finisher.o -lrt -lpthread

initializer.o: initializer.c
	gcc -Wall -g -c initializer.c -o initializer.o -lrt -lpthread

run:
	gcc -Wall initializer.c -o initializer.o -lrt -lpthread
	gcc -Wall read.c -o read.o -lrt -lpthread
	gcc -Wall writter.c -o writter.o -lrt -lpthread
	gcc -Wall finisher.c -o finisher.o -lrt -lpthread
	./main.o

clean:
	rm *.o
	rm /dev/shm/*
