#makefile

write: write.o
		gcc -Wall -g write.o -o write -lrt

read: read.o
		gcc -Wall -g read.o -o read -lrt

write.o: write.c
		gcc -g -Wall -c write.c -o write.o -lrt

read.o: read.c
		gcc -g -Wall -c read.c -o read.o -lrt

clean:
		rm -f *.o write read

run: write read
		./write
		./read
