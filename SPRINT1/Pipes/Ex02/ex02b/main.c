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

typedef struct {
 char str[80];
 int num;
} data;

int main(void){

  /*data para leitura*/
  data dLeitura;

  /*data para Escrita*/
  data dEscrita;

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
    scanf("%d", &dEscrita.num);

    /* String pedido no enunciado */
    printf("Please insert the String:");
    while (fgets(dEscrita.str,sizeof(dEscrita.str),stdin) != NULL){
      if (qtd > 0) {
        break;
      }
      qtd++;
    }

    /* escreve a struct no pipe */
    write(fd[1], &dEscrita , sizeof(dEscrita));

    /* fecha a extremidade de escrita */
    close(fd[1]);

  } else {

    /* fecha a extremidade não usada, filho só lê */
    close(fd[1]);

    /* lê a struct do pipe */
    read(fd[0], &dLeitura, sizeof(dLeitura));
    printf("\nFilho leu o inteiro: %d", dLeitura.num);
    printf("\nFilho leu a string: %s\n",  dLeitura.str);

    /* fecha a extremidade de leitura */
    close(fd[0]);
    exit(1);
  }

  return 0;
}
