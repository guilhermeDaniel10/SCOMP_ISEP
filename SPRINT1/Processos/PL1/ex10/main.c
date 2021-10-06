#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

int lenght = 2000;
int qtd_children = 10;
int n = 42;

void array2MilRandom(int *array);

int search(int posInit, int posFin, int *array);

int main(void){

  int array[lenght];
  memset(array,0,sizeof(array)); /* Inicializa o array em todas as posições a 0 */
  array2MilRandom(array);

  int validPositions[qtd_children];
  memset(validPositions,0,sizeof(validPositions));

  int status, posInit = 0, posFin = lenght/qtd_children;

  for (int i = 0; i < qtd_children; i++) {
    pid_t p = fork();
    if (p != 0) {
      waitpid(p,&status,0);
      if (WIFEXITED(status)) {
        posInit = posFin;
        posFin = posFin + lenght/qtd_children;
        validPositions[i] = WEXITSTATUS(status);
      }
    } else {
      int result;
      result = search(posInit, posFin, array);
      exit(result);
    }
  }

  for (int i = 0; i < 10; i++) {
    if (validPositions[i]!=255) {
      printf("O Número %d foi encontrado na posição %d\n",n,validPositions[i]);
    }
  }

}

void array2MilRandom(int *array){
  int upper = 1000;
  int lower = 0;

  for(int i = 0; i < lenght; i++){
    array[i] = (int) (rand() % (upper - lower + 1)) + lower;
    /*  printf("ARRAY[%d]=%d\n",i,array[i]);  */
  }
}

int search(int posInit, int posFin, int *array){

  for (int i = posInit; i < posFin; i++) {
    if (array[i] == n) {
      return i;
    }
  }

  return 255; /*Numero nao encontrado */
}
