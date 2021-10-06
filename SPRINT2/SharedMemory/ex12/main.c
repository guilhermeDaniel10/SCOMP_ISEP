#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For constants mode */
#include <fcntl.h> /* For constants O_* */

#define STR_SIZE 50
#define NR_DISC 10

typedef struct{
  int numero;
  char nome[STR_SIZE];
  int disciplinas[NR_DISC];
  int canRead;
}aluno;

void highest(int *disciplinas){
  int i = 0, highest = disciplinas[i];
  for (i = 1; i < NR_DISC; i++) {
      if (highest < disciplinas[i]) {
        highest = disciplinas[i];
      }
  }
  printf("\nThe highest grade was %d", highest);
}

void lowest(int *disciplinas){
  int i = 0, lowest = disciplinas[i];
  for (i = 1; i < NR_DISC; i++) {
      if (lowest > disciplinas[i]) {
        lowest = disciplinas[i];
      }
  }
  printf("\nThe lowest grade was %d", lowest);
}

void average(int *disciplinas){
  int i, sum = 0;
  for (i = 0; i < NR_DISC; i++) {
    sum += disciplinas[i];
  }
  printf("\nThe average grade was %.2f", (float) sum/NR_DISC );
}

void showInfo(aluno *shared_data){
  printf("\nNumber:%d\nNome:%s\n",shared_data->numero, shared_data->nome);
  for (int i = 0; i < NR_DISC; i++) {
    printf("Grade for %d = %d\n",i+1,shared_data->disciplinas[i]);
  }
}

int main(int argc, char *argv[]) {
  /*
  File descriptor.
  */
 int fd;
 /*
 Size of the struct being shared.
 */
 int data_size = sizeof(aluno);
 /*
 Allows to request the area to be mapped in a specific address.
 */
 aluno *shared_data;
 /*
 Creates (O_CREAT) shared memory area (with error if existis because of O_EXCL)
 with nome “/share”, and read/write permissons (O_RDWR), and open for user to
 read (S_IRUSR) and write (S_IWUSR).
 */
 fd = shm_open("/shared_InfoAluno", O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR);
 /*
 Defines the size of the area(struct aluno) and initializes it to zero
 */
 ftruncate (fd, data_size);
 /*
 Shares the mapped area to the pointer
 */
 shared_data = (aluno *)mmap(NULL,data_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
 /* The writer must be executed before the reader */
 shared_data->canRead = 0;

 /* Creation of two processes */
 int pid = fork();

  if (pid == -1) {
       perror("fork failed");
       exit(EXIT_FAILURE);
  }
  /* writer*/
  if (pid > 0) {
        int qtd = 0;

        printf("Student Number: ");
        scanf("%d", &(shared_data->numero));

        printf("Enter the Student Nome:");
        while (fgets(shared_data->nome, STR_SIZE, stdin) != NULL){
          if (qtd > 0) {
            break;
          }
          qtd++;
        }
        for (int i = 0; i < NR_DISC; i++) {
          printf("Enter the classification for the classe %d out of %d: ",i+1,NR_DISC);
          scanf("%d", &(shared_data->disciplinas[i]));
        }

        shared_data->canRead = 1;
    /*WAITS for son, now tests pipes */
    wait(NULL);
  /* reader*/
  } else {
        while(shared_data->canRead == 0){}
        shared_data->canRead = 0;
        showInfo(shared_data);
        highest(shared_data->disciplinas);
        lowest(shared_data->disciplinas);
        average(shared_data->disciplinas);
    /* disconnects */
    fd = munmap(shared_data, data_size);
    /* Check error */
    if (fd < 0) exit(1);
    /* removes the shared parte*/
    fd = shm_unlink("/shared_InfoAluno");
    /* Check error */
    if (fd < 0) exit(1);
    exit(0);
  }
  printf("\nThe end!\n");
  /* disconnects */
  fd = munmap(shared_data, data_size);
  /* Check error */
  if (fd < 0) exit(1);
  /* closes pipe */
  fd = close(fd);
  /* Check error */
  if (fd < 0) exit(1);
  exit(0);
}
