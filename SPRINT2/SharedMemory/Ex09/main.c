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
  int new_data;
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

  int fd, data_size = sizeof(num);
  num *shared;

  /*Abre a memória partilhada */
  fd = shm_open("/ex05", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);

  if(fd == -1){
    printf("Erro a abrir");
    exit(EXIT_FAILURE);
  }


  ftruncate(fd, data_size);

  /*Obtem o pointer da estrutura da memoria partilhada */
  shared = (num *)mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

  pid_t pid;
  int i = 0;
  int arr[1000];

  fill_array(arr);

  for(i = 0; i < 10; i++){
    pid = fork();

    if(pid == -1){
      perror("Erro no fork");
      return -1;
    }

    if(pid == 0){
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

    int l = 0;
    /*Ordena o array de modo a que a posicao dos valores esteja de acordo com o index */
    for(l = 0; l < 10; l++){
      if(l == i){
        shared->num[i] = max;
      }
    }

    printf("Maior valor na porção %d: %d\n", i, shared->num[i]);

    if (munmap(shared, data_size) < 0) {
        printf("Erro no munmap\n");
        exit(EXIT_FAILURE);
    }

    exit(1);
  } else {
    wait(NULL);
  }
}

int maxValue = -1;

int ind = 0;

/*Determina o maximo do array com os maximos locais */
for(ind = 0; ind < 10; ind++){
  if(shared->num[ind] > maxValue){
    maxValue = shared->num[ind];
  }
}

printf("O maior valor é: %d\n", maxValue);


if(shm_unlink("/ex05") < 0){
  printf("Sem necessidade de fechar.\n");
  exit(EXIT_FAILURE);
}

  if (munmap(shared, data_size) < 0) {
      printf("Erro no munmap\n");
      exit(EXIT_FAILURE);
  }

  if (close(fd) < 0) {
      printf("Erro no close\n");
      exit(EXIT_FAILURE);
  }












  return 0;
}
