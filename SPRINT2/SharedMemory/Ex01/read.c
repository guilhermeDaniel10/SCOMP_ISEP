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
  int num;
  char name[256];
} student;

int main(int argc, char *argv[]){

  int data_size = sizeof(student);
  student *aluno;
  /*Cria e abre a area de memoria */
  int fd = shm_open("/shmtest", O_RDWR, S_IRUSR|S_IWUSR);

  /*Define o tamanho */
  ftruncate(fd, data_size);


  aluno = (student *)mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
  printf("Numero do aluno: %d\n", aluno->num);
  printf("Nome do aluno: %s\n", aluno->name);



  if (munmap((void *)aluno, data_size) < 0) {
      printf("Erro no munmap\n");
      exit(EXIT_FAILURE);
  }

  if (close(fd) < 0) {
      printf("Error no close\n");
      exit(EXIT_FAILURE);
  }


  return EXIT_SUCCESS;



}
