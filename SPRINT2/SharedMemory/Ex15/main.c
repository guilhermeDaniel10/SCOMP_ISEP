#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For constants mode */
#include <fcntl.h> /* For constants O_* */

typedef struct{
  int integersToStore[10];
  int realIndex;
  int maxIndex;
  int stop;
  int canWrite;
  int canRead;
} circularBuffer;

int main(int argc, char *argv[]) {
  shm_unlink("/shared_InfocircularBuffer");
  /*
  File descriptor.
  */
 int fd;
 /*
 Size of the struct being shared.
 */
 int data_size = sizeof(circularBuffer);
 /*
 Allows to request the area to be mapped in a specific address.
 */
 circularBuffer *shared_data;
 /*
 Creates (O_CREAT) shared memory area (with error if existis because of O_EXCL)
 with nome “/share”, and read/write permissons (O_RDWR), and open for user to
 read (S_IRUSR) and write (S_IWUSR).
 */
 fd = shm_open("/shared_InfocircularBuffer", O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR);
 /*
 Defines the size of the area(struct circularBuffer) and initializes it to zero
 */
 ftruncate (fd, data_size);
 /*
 Shares the mapped area to the pointer
 */
 shared_data = (circularBuffer *)mmap(NULL,data_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
 /* The writer must be executed before the reader */

 memset( shared_data->integersToStore,0, sizeof( shared_data->integersToStore));
 shared_data->canRead = 0;
 shared_data->canWrite = 1;
 shared_data->maxIndex = 9;
 shared_data->realIndex = 0;
 shared_data->stop = 0;

 /* Creation of two processes */
 int pid = fork();

  if (pid == -1) {
       perror("fork failed");
       exit(EXIT_FAILURE);
  }
  /*Producer*/
  if (pid > 0) {
    int rnd = 0;
    while(rnd < 3){
          printf("--------------------Round = %d----------------------\n", rnd+1);
          do {
            while(shared_data->canWrite == 0){}

            printf("\n---Input a Incremental Value for index %d: ",shared_data->realIndex);
            scanf("%d", &shared_data->integersToStore[shared_data->realIndex]);
            shared_data->canWrite = 0;
            shared_data->canRead = 1;
          } while(shared_data->realIndex < shared_data->maxIndex);
        shared_data->realIndex=0;
        rnd++;
    }
    shared_data->stop = 1;
    /*WAITS for son, now tests pipes */
    wait(NULL);
  /* reader*/
  } else {
    while(shared_data->stop == 0){

        while(shared_data->canRead == 0){}

              if (shared_data->realIndex != 0 && shared_data->integersToStore[shared_data->realIndex] < shared_data->integersToStore[shared_data->realIndex-1] ) {
                  printf("-Not a Incremental value receiced this was %d, but the previous was %d!!!\n", shared_data->integersToStore[shared_data->realIndex], shared_data->integersToStore[shared_data->realIndex-1]);
                  fflush(stdout);
              } else if (shared_data->realIndex == 0 && shared_data->integersToStore[shared_data->maxIndex] != 0) {
                  printf("-Not a Incremental value receiced this was %d, but the previous was %d!!!", shared_data->integersToStore[shared_data->realIndex], shared_data->integersToStore[shared_data->maxIndex]);
                  fflush(stdout);
              } else if (shared_data->realIndex == 0 && shared_data->integersToStore[shared_data->maxIndex] == 0) {
                printf("+++Incremental value receiced was %d!!!", shared_data->integersToStore[shared_data->realIndex]);
                fflush(stdout);
                shared_data->realIndex++;
              } else {
                printf("+++Incremental value receiced was %d!!!", shared_data->integersToStore[shared_data->realIndex]);
                fflush(stdout);
                shared_data->realIndex++;
              }
              shared_data->canWrite = 1;
              shared_data->canRead = 0;
    }
    /* disconnects */
    fd = munmap(shared_data, data_size);
    /* Check error */
    if (fd < 0) exit(1);
    /* removes the shared parte*/
    fd = shm_unlink("/shared_InfocircularBuffer");
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
