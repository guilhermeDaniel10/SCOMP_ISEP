#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

typedef struct {
  int num[10];
} shared_data_type;

int main(int argc, char *argv[]){

  int data_size = sizeof(shared_data_type);
  shared_data_type *shared_data;
  /*Cria e abre a area de memoria */
  int fd = shm_open("/shmtest", O_RDWR, S_IRUSR|S_IWUSR);

  /*Define o tamanho */
  ftruncate(fd, data_size);


  shared_data = (shared_data_type *)mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

  int i = 0;
  for(i = 0; i < 10; i++){
    printf("Numero: %d\n", shared_data->num[i]);
  }

  if (munmap((void *)shared_data, data_size) < 0) {
      printf("Erro no munmap\n");
      exit(EXIT_FAILURE);
  }

  if (close(fd) < 0) {
      printf("Error no close\n");
      exit(EXIT_FAILURE);
  }


  return EXIT_SUCCESS;



}
