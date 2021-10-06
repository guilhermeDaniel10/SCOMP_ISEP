#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For constants mode */
#include <fcntl.h> /* For constants O_* */

#define SIZE_DESCRIPTION 30

typedef struct {
  int productCode;
  char description[SIZE_DESCRIPTION];
  float price;
  int canRead;
  int canWrite;
} shared_data_product;

int main(int argc, char *argv[]) {

    int fd, data_size = sizeof(shared_data_product);

    shared_data_product *shared_data;

    fd = shm_open("/shared_Info", O_EXCL|O_RDWR,S_IRUSR|S_IWUSR);
    if (fd == -1) {
        printf("Error at shm_open() Leitor!\n");
        exit(1);
    }

    ftruncate (fd, data_size);

    shared_data = (shared_data_product *)mmap(NULL,data_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

    while(shared_data->canRead == 0){}
      printf("\n---> Entered product code: %d",shared_data->productCode);
      printf("\n---> Entered product description: %s",shared_data->description);
      printf("---> Entered product price: %.2f€\n",shared_data->price);

    fd = munmap(shared_data, data_size); /* disconnects */
    if (fd < 0) exit(1); /* Check error */

    fd = shm_unlink("/shared_Info");
    if (fd < 0) exit(1); /* Check error */
    exit(0);
}
