#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/time.h>   // for gettimeofday()
#include <unistd.h>

#define TAMANHO 10000
#define NBR_CHILDS 10
int divisions = TAMANHO/NBR_CHILDS;

/* Estrutura que armazena o array com os 10 numeros com o maximo local das
diferentes porcoes, com o new_data para determinar a paragem de escrever*/
typedef struct{
  int num[10];
  int new_data;
} num;

struct timeval start_t;
struct timeval end_t;

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
  int arr[TAMANHO];

  fill_array(arr);
  gettimeofday(&start_t, NULL);

  for(i = 0; i < NBR_CHILDS; i++){
    pid = fork();

    if(pid == -1){
      perror("Erro no fork");
      return -1;
    }

    if(pid == 0){
    /*Primeiro index do ciclo */
    int start = i * divisions;
    /*Ultimo index do ciclo */
    int final = start + divisions;

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
    for(l = 0; l < NBR_CHILDS; l++){
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
for(ind = 0; ind < NBR_CHILDS; ind++){
  if(shared->num[ind] > maxValue){
    maxValue = shared->num[ind];
  }
}

printf("O maior valor é: %d\n", maxValue);
gettimeofday(&end_t, NULL);
long seconds = (end_t.tv_sec - start_t.tv_sec);
long micros = ((seconds * 1000000) + end_t.tv_usec) - (start_t.tv_usec);


printf("----------Demorou:  %ld segundos e %ld micro segundos com %d filhos\n", seconds, micros, NBR_CHILDS);


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
