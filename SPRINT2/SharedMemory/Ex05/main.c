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

#define TAMANHO 1000000

typedef struct{
  int num1;
  int num2;
} nums;

int main(){



  int fd, data_size = sizeof(nums);

  nums *shared;

  fd = shm_open("/ex05", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);

  if(fd == -1){
    printf("Erro a abrir");
    exit(EXIT_FAILURE);
  }

  ftruncate(fd, data_size);

  shared = (nums *)mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

  shared->num1 = 8000;
  shared->num2 = 200;

  pid_t pid;

  pid = fork();

  if(pid == -1){
    perror("Erro no fork");
    return -1;
  }


  if(pid > 0){
    int i = 0;
    for(i = 0; i < TAMANHO; i++){
      shared->num1 += 1;
      shared->num2 -= 1;
    }


  } else {
    int i = 0;
    for(i = 0; i < TAMANHO; i++){
      shared->num1 -= 1;
      shared->num2 += 1;
    }


    if (munmap(shared, data_size) < 0) {
        printf("Erro no munmap\n");
        exit(EXIT_FAILURE);
    }

    if(shm_unlink("/ex05") < 0){
      printf("Sem necessidade de fechar.\n");
      exit(EXIT_FAILURE);
    }

    exit(1);

  }
  // Os resultados nem sempre estão corretos
  printf("Numero 1: %d\n", shared->num1);
  printf("Numero 2: %d\n", shared->num2);


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
