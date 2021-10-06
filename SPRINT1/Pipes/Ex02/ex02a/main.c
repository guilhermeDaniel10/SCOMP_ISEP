/* libraria do printf */
#include <stdio.h>
/* librarias para os processos */
#include <sys/types.h> /*pid_t */
#include <unistd.h> /*fork and pipe*/
/*librarias do wait */
#include <sys/types.h>
#include <sys/wait.h>
/*libraria strlen*/
#include <string.h>
#include <stdlib.h>


int BUFFER_SIZE = 80;

int main(void){

  /*inteiro para leitura*/
  int intLeitura = 0;
  /*string para a leitura*/
  char read_msg[BUFFER_SIZE];

  /*inteiro para Escrita*/
  int intEscrita = 0;
  /*string para Escrita*/
  char write_msg[BUFFER_SIZE];

  /*Pipe*/
  int fd[2];

  /*Process Identifier*/
  pid_t pid;

  /* cria o pipe */
  if(pipe(fd) == -1){
    perror("Pipe failed");
    return 1;
  }

  pid = fork();

  /*no caso de não conseguir fazer mais forks*/
  if(pid == -1){
    perror("Fork falhou!");
    exit(1);
  }

  if(pid > 0){

    /* fecha a extremidade não usada pai só escreve */
    close(fd[0]);
    int qtd = 0;

    /* Inteiro pedido no enunciado */
    printf("\nPlease insert the Integer:");
    scanf("%d", &intEscrita);

    /* escreve no pipe */
    write(fd[1], &intEscrita, sizeof(intEscrita));
    printf("Please insert the String:");
    while (fgets(write_msg,BUFFER_SIZE,stdin) != NULL){
      if (qtd > 0) {
        break;
      }
      qtd++;
    }

    /* escreve no pipe */
    write(fd[1], write_msg, sizeof(write_msg));

    /* fecha a extremidade de escrita */
    close(fd[1]);

  } else {

    /* fecha a extremidade não usada, filho só lê */
    close(fd[1]);

    /* lê inteiro do pipe */
    read(fd[0], &intLeitura, sizeof(intLeitura));
    printf("\nFilho leu o inteiro: %d", intLeitura);

    /* lê string do pipe */
    read(fd[0], read_msg, sizeof(read_msg));
    printf("\nFilho leu a string: %s\n", read_msg);

    /* fecha a extremidade de leitura */
    close(fd[0]);
    exit(1);
  }

  return 0;
}
