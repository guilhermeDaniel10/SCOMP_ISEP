#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For constants mode */
#include <fcntl.h> /* For constants O_* */

#define SIZE_ARRAY 100

typedef struct {
  char arrToShare[SIZE_ARRAY];
  int canRead;
  int canWrite;
} shared_data_array;


void randomChar(char *arrToShare){
  int i;
  int upper = 90;
  int lower = 65;
    for (i = 0; i < SIZE_ARRAY; i++) {
      arrToShare[i] = (int) (rand() % (upper - lower + 1)) + lower;
    }
}

int main(int argc, char *argv[]) {

      int fd, data_size = sizeof(shared_data_array);

      shared_data_array *shared_data;

      fd = shm_open("/shared_Info", O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR);

      if (fd == -1) {
          printf("Error at SHM_OPEN() Escritor!\n");
          exit(1);
      }

      ftruncate(fd, data_size);

      shared_data = (shared_data_array*)mmap(NULL,data_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

      shared_data->canRead = 0;
      shared_data->canWrite = 1;

      while (shared_data->canWrite == 1) {
          randomChar(shared_data->arrToShare);
          shared_data->canWrite = 0;
          shared_data->canRead = 1;
      }

      fd = munmap(shared_data, data_size); /* disconnects */
      if (fd < 0) exit(1); /* Check error */

      fd = close(fd);
      if (fd < 0) exit(1); /* Check error */
      exit(0);

}
