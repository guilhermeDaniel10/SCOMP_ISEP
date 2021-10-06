escritor: escritor.o
		gcc -Wall -g escritor.o -o escritor -lrt

leitor: leitor.o
		gcc -Wall -g leitor.o -o leitor -lrt

escritor.o: escritor.c
		gcc -g -Wall -c escritor.c -o escritor.o -lrt

leitor.o: leitor.c
		gcc -g -Wall -c leitor.c -o leitor.o -lrt

clean:
		rm -f *.o escritor leitor

run: escritor leitor
		./escritor
		./leitor
