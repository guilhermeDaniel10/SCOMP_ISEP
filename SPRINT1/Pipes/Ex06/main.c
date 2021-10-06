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

int lenght = 1000;
int nbrChilds = 5;

int sumArray(int *array, int posInit, int posFin);
void preencherArray(int *array);

int main(void){
  /*Integer array asked in the exercise*/
  int vec1[lenght];
  int vec2[lenght];
  /*Fills the arrays with 1000 elements*/
  preencherArray(vec1);
  preencherArray(vec2);
  /*result array for the father*/
  int result[nbrChilds];
  /*Integer for reading*/
  int readNum;
  /*Final sum*/
  int finalSum = 0;

  /*Pipe*/
  int fd[2];

  /*Process Identifier*/
  pid_t pid;
  int i;

  /* creates pipe */
  if(pipe(fd) == -1){
    perror("Pipe failed");
    return 1;
  }

  for (i = 0; i < nbrChilds; i++) {
      pid = fork();

      /*no caso de não conseguir fazer mais forks*/
      if(pid == -1){
        perror("Fork falhou!");
        exit(1);
      }

       if(pid == 0) {
        /* fecha a extremidade não usada, filhos só escrevem */
        close(fd[0]);
        int posFin = (i+1)*200;
        int posInit = i*200;

        int sumVec1 = sumArray(vec1, posInit ,posFin);
        int sumVec2 = sumArray(vec2, posInit ,posFin);

        int tmp = sumVec1 + sumVec2;

        /* escreve inteiros no pipe */
        write(fd[1], &tmp, sizeof(tmp));

        /* fecha a extremidade de escrita */
        close(fd[1]);
        exit(1);
      }
  }

    /* fecha a extremidade não usada pai só lê */
    close(fd[1]);
    int pos = 0;

    /* lê os inteiros dos filhos pelo pipe */
    while(read(fd[0], &readNum, sizeof(readNum)) > 0){
      *(result+pos) = readNum;
      pos++;
    }
    /*Waits for all the children*/
    while ((pid = wait(NULL)) >= 0);
    close(fd[0]);

  /*Outputs the sum*/
  for (int x = 0; x < nbrChilds; x++) {
    finalSum += result[x];
    printf("\n---Soma dos filhos posição %d = %d\n", x+1,result[x]);
  }
  printf("\nSoma final: %d", finalSum);

  return 0;
}

/*função para preencher array com 1000 elementos*/
void preencherArray(int *array){
  int upper = 1000;
  int lower = 0;

  for(int i = 0; i < lenght; i++){
    array[i] = (int) (rand() % (upper - lower + 1)) + lower;
  }
}

/*função soma no intervalo desejado retornando esse numero*/
int sumArray(int *array, int posInit, int posFin){
  int sum = 0;

  for(int i = posInit; i < posFin; i++){
    sum += array[i];
  }

  return sum;
}
