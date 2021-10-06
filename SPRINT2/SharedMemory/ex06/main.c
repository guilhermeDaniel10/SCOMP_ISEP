#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For constants mode */
#include <fcntl.h> /* For constants O_* */
#include <sys/time.h>   // for gettimeofday()

#define SIZE_ARRAY 1000
int arrToShare[SIZE_ARRAY];

typedef struct {
  int transfer;
  struct timeval start_t;
  struct timeval end_t;
  int canRead;
  int canWrite;
} shared_data_array;

/* Filles with 1.000.000 numbers a array */
void randomArray(){
  int i;
  int upper = 1000;
  int lower = 0;
    for (i = 0; i < SIZE_ARRAY; i++) {
      arrToShare[i] = (int) (rand() % (upper - lower + 1)) + lower;
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
 int data_size = sizeof(shared_data_array);
 /*
 Allows to request the area to be mapped in a specific address.
 */
 shared_data_array *shared_data;
 /*
 Creates (O_CREAT) shared memory area (with error if existis because of O_EXCL)
 with name “/share”, and read/write permissons (O_RDWR), and open for user to
 read (S_IRUSR) and write (S_IWUSR).
 */
 fd = shm_open("/shared_Info", O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR);
 /*
 Defines the size of the area(struct shared_data_array) and initializes it to zero
 */
 ftruncate (fd, data_size);
 /*
 Shares the mapped area to the pointer
 */
 shared_data = (shared_data_array *)mmap(NULL,data_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
 /* The writer must be executed before the reader */
 shared_data->canRead = 0;
 shared_data->canWrite = 1;
 printf("\nSHARED MEMORY!\n");
 /* Creation of two processes */
 int pid = fork();

  if (pid == -1) {
       perror("fork failed");
       exit(EXIT_FAILURE);
  }
  /* writer*/
  if (pid > 0) {
    int elems = 0;
    randomArray();
    gettimeofday(&shared_data->start_t, NULL);
    while (elems != SIZE_ARRAY) {
      elems++;
        while (shared_data->canWrite == 0){}
          shared_data->transfer = arrToShare[elems];
          //printf("\n---------------------->shared %d/%d = %d\n",elems,SIZE_ARRAY,shared_data->transfer);
          shared_data->canWrite = 0;
          shared_data->canRead = 1;
    }
    /*WAITS for son, now tests pipes */
    wait(NULL);
  /* reader*/
  } else {
    int elems = 0;
    while (elems != SIZE_ARRAY) {

        while(shared_data->canRead == 0){}
        arrToShare[elems] = shared_data->transfer;
        //printf("\n!!!!!!!!!!!!!!!!!!!received %d/%d = %d\n",elems+1,SIZE_ARRAY,shared_data->transfer);
        shared_data->canWrite = 1;
        shared_data->canRead = 0;
        elems++;
    }

    gettimeofday(&shared_data->end_t, NULL);

    long seconds = (shared_data->end_t.tv_sec - shared_data->start_t.tv_sec);
    long micros = ((seconds * 1000000) + shared_data->end_t.tv_usec) - (shared_data->start_t.tv_usec);
    printf("\nTotal time taken was %ld seconds and %ld microseconds\n", seconds, micros);

    /* disconnects */
    fd = munmap(shared_data, data_size);
    /* Check error */
    if (fd < 0) exit(1);
    /* removes the shared parte*/
    fd = shm_unlink("/shared_Info");
    /* Check error */
    if (fd < 0) exit(1);
    exit(0);
  }
  /* disconnects */
  fd = munmap(shared_data, data_size);
  /* Check error */
  if (fd < 0) exit(1);
  /* closes pipe */
  fd = close(fd);
  /* Check error */
  if (fd < 0) exit(1);

   printf("\nPIPES!\n");
   shared_data_array sharePipes;

   /*Pipe*/
   int fd2[2];
  /* creates pipe */
  if(pipe(fd2) == -1){
    perror("Pipe failed");
    return 1;
  }

  pid = fork();
  /*no caso de não conseguir fazer mais forks*/
  if(pid == -1){
    perror("Fork falhou!");
    exit(1);
  }

  if(pid > 0){

    /* fecha a extremidade não usada pai só escreve */
    close(fd2[0]);
    gettimeofday(&sharePipes.start_t, NULL);
    for (int i = 0; i < SIZE_ARRAY; i++) {
        sharePipes.transfer = arrToShare[i];
        //printf("\n---------------------->shared %d/%d = %d\n",i+1,SIZE_ARRAY,sharePipes.transfer);
        /* escreve no pipe */
        write(fd2[1], &sharePipes, sizeof(sharePipes));
    }
    /* fecha a extremidade de escrita */
    close(fd2[1]);


  }else{
    /* fecha a extremidade não usada, filho só lê */
    close(fd2[1]);

    /* lê os inteiros dos filhos pelo pipe */
    while(read(fd2[0], &sharePipes, sizeof(sharePipes))){
      //printf("\n!!!!!!!!!!!!!!!!!!!received %d/%d = %d\n",pos,SIZE_ARRAY,sharePipes.transfer);
    }

    gettimeofday(&sharePipes.end_t, NULL);

    long seconds = (sharePipes.end_t.tv_sec - sharePipes.start_t.tv_sec);
    long micros = ((seconds * 1000000) + sharePipes.end_t.tv_usec) - (sharePipes.start_t.tv_usec);
    printf("\nTotal time taken was %ld seconds and %ld microseconds\n", seconds, micros);

    /* fecha a extremidade de leitura */
    close(fd2[0]);
    exit(1);
  }

  wait(NULL);
  printf("\nThe end!\n");
  exit(0);
}
