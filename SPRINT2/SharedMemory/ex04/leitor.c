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

float randomChar(char *arrToShare){
  int i, sum = 0;
    for (i = 0; i < SIZE_ARRAY; i++) {
      printf("\n%dº -> %c -> %d",i+1,arrToShare[i], (int) arrToShare[i]);
      sum += (int) arrToShare[i];
    }
    return (sum/SIZE_ARRAY);
}

int main(int argc, char *argv[]) {

    int fd, data_size = sizeof(shared_data_array);

    shared_data_array *shared_data;

    fd = shm_open("/shared_Info", O_EXCL|O_RDWR,S_IRUSR|S_IWUSR);
    if (fd == -1) {
        printf("Error at shm_open() Leitor!\n");
        exit(1);
    }

    ftruncate (fd, data_size);

    shared_data = (shared_data_array *)mmap(NULL,data_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

    while(shared_data->canRead == 0){}
      float average = randomChar(shared_data->arrToShare);
      printf("\nAverage=%.2f\n",average);

    fd = munmap(shared_data, data_size); /* disconnects */
    if (fd < 0) exit(1); /* Check error */

    fd = shm_unlink("/shared_Info");
    if (fd < 0) exit(1); /* Check error */
    exit(0);
}
