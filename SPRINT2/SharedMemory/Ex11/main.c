#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define TAMANHO 1000

/* Estrutura que armazena o array com os 10 numeros com o maximo local das
diferentes porcoes, com o new_data para determinar a paragem de escrever*/
typedef struct{
  int num[10];
} num;

/*Preenche um array com numeros aleatorios */
void fill_array(int *arr){
  srand(time(NULL));
  int j = 0;
  for(j = 0; j < TAMANHO; j++){
    arr[j] = rand()% 1000 + 1;
  }
}

int main(){

  int fd[2];

  pid_t pid;
  int i = 0;
  int arr[1000];

  if(pipe(fd) == -1){
    perror("Erro");
  }

  fill_array(arr);

  for(i = 0; i < 10; i++){
    pid = fork();


    if(pid == -1){
      perror("Erro no fork");
      return -1;
    }

    if(pid == 0){
    close(fd[0]);
    /*Primeiro index do ciclo */
    int start = i * 100;
    /*Ultimo index do ciclo */
    int final = start + 100;

    int k = 0;
    int max = -1;

    /*Determina o maximo local da range de numeros no determinado processo */
    for(k = start; k < final; k++){
      if(arr[k] > max){
        max = arr[k];
      }
    }
    printf("Maior valor local: %d\n", max);
    write(fd[1], &max, sizeof(max));
    close(fd[1]);

    exit(1);
  } else {
    wait(NULL);
  }
}

int maxValue = -1;
int tmpArr[10];
int index = 0;
int read_msg;

close(fd[1]);
while(read(fd[0], &read_msg, sizeof(read_msg)) > 0){
  tmpArr[index] = read_msg;
  index++;
}
close(fd[0]);

int ind = 0;
for(ind = 0; ind < 10; ind++){
  if(tmpArr[ind] > maxValue){
    maxValue = tmpArr[ind];
  }
}

printf("O maior valor é: %d\n", maxValue);


  return 0;
}
