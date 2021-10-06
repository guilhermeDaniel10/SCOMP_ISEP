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

typedef struct {
  int num[10];
} shared_data_type;

int main(int argc, char *argv[]){

  /*Define o tamanho */
  int data_size = sizeof(shared_data_type);

  shm_unlink("/shmtest");
  int fd= shm_open("/shmtest", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);

  ftruncate(fd, data_size);

  /*Cria e abre a area de memoria */
  shared_data_type *shared_data;



  if(fd == -1){
    printf("Erro a abrir");
    exit(0);
  }



  /*Mapeia a area partilhada no espaco de enderecamento do processo */
  shared_data = (shared_data_type*)mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
  int i = 0;
  srand(time(NULL));

  for(i = 0; i < 10; i++){
    shared_data->num[i] = rand()%((20+1)-1) + 1;
  }

  if (munmap((void *)shared_data, data_size) < 0) {
          printf("Erro no munmap\n");
          exit(EXIT_FAILURE);
  }

  if (close(fd) < 0) {
         printf("Erro no close\n");
         exit(EXIT_FAILURE);
     }


     return EXIT_SUCCESS;
}
