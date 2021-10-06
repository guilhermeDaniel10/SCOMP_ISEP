#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For constants mode */
#include <fcntl.h> /* For constants O_* */

#define SIZE 10000

typedef struct {
  int transfer;
  int canRead;
  int canWrite;
} share_num;

int main(int argc, char *argv[]) {
  /*
  File descriptor.
  */
 int fd;
 /*
 Size of the struct being shared.
 */
 int data_size = sizeof(share_num);
 /*
 Allows to request the area to be mapped in a specific address.
 */
 share_num *shared_data;
 /*
 Creates (O_CREAT) shared memory area (with error if existis because of O_EXCL)
 with name “/share”, and read/write permissons (O_RDWR), and open for user to
 read (S_IRUSR) and write (S_IWUSR).
 */
 fd = shm_open("/shared_Info", O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR);
 /*
 Defines the size of the area(struct share_num) and initializes it to zero
 */
 ftruncate (fd, data_size);
 /*
 Shares the mapped area to the pointer
 */
 shared_data = (share_num *)mmap(NULL,data_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
 /* The writer must be executed before the reader */
 shared_data->transfer = 100;
 shared_data->canRead = 0;
 shared_data->canWrite = 1;

 /* Creation of two processes */
 int pid = fork();

  if (pid == -1) {
       perror("fork failed");
       exit(EXIT_FAILURE);
  }
  /* writer*/
  if (pid > 0) {
    int elems = 0;
    while (elems != SIZE) {
      elems++;
        while (shared_data->canWrite == 0){}
          shared_data->transfer++;
          printf("---Number On Parent -> %d\n", shared_data->transfer);
          shared_data->canWrite = 0;
          shared_data->canRead = 1;
    }
    /*WAITS for son, now tests pipes */
    wait(NULL);
  /* reader*/
  } else {
    int elems = 0;
    while (elems != SIZE) {
        while(shared_data->canRead == 0){}
        shared_data->transfer--;
        printf("------Number On Child -> %d\n", shared_data->transfer);
        shared_data->canWrite = 1;
        shared_data->canRead = 0;
        elems++;
    }
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
  printf("Number -> %d\n", shared_data->transfer);
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
