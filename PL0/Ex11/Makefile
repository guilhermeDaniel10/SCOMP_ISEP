#makefile intuitivo
#variaveis do makefile
NAME = main
INCLUDES =
SOURCES = ${NAME}.c
OBJFILES = ${NAME}.o
EXE = ${NAME}

#Regras do sufixo
.SUFFIXES : .c .o
${OBJFILES}: ${SOURCES} ${INCLUDES}
.c.o:
		gcc -g -ansi -Wall -c $<
		
all: ${EXE}

#para o programa principal
${EXE} : ${OBJFILES}
		gcc -g -o ${EXE} ${OBJFILES}
#executar o programa principal
run: ${EXE}
		./${EXE}

#limpar
clean:
		rm ${OBJFILES} ${EXE}
