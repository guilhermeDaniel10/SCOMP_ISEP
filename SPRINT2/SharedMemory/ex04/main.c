#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For constants mode */
#include <fcntl.h> /* For constants O_* */

#define SIZE_DESCRIPTION 15

typedef struct {
  int productCode;
  char description[SIZE_DESCRIPTION];
  float price;
  int canRead;
  int canWrite;
} shared_data_product;

int main(int argc, char *argv[]) {
  /*
  File descriptor.
  */
 int fd;
 /*
 Size of the struct being shared.
 */
 int data_size = sizeof(shared_data_product);
 /*
 Allows to request the area to be mapped in a specific address.
 */
 shared_data_product *shared_data;
 /*
 Creates (O_CREAT) shared memory area (with error if existis because of O_EXCL)
 with name “/share”, and read/write permissons (O_RDWR), and open for user to
 read (S_IRUSR) and write (S_IWUSR).
 */
 fd = shm_open("/share", O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR);
 /*
 Defines the size of the area(struct shared_data_product) and initializes it to zero
 */
 ftruncate (fd, data_size);
 /*
 Shares the mapped area to the pointer
 */
 shared_data = (shared_data_product *)mmap(NULL,data_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
 /* The writer must be executed before the reader */
 shared_data->canRead = 0;
 shared_data->canWrite = 1;
 /* Creation of two processes */
 int pid = fork();
  /* writer*/
  if (pid > 0) {
        while (shared_data->canWrite == 1) {
          printf("---------------->MAIN1");
          printf("Enter product code: ");
          scanf("%d", &(shared_data.productCode));

          printf("Enter the product description: ");
          scanf("%s", &(shared_data.price));

          printf("Enter the product price: ");
          scanf("%f", &(shared_data.description));

          shared_data->canWrite = 0;
          shared_data->canRead = 1;
        }
//wait();
  /* reader*/
  } else {
    while(shared_data->canRead == 0);
    printf("---------------->MAIN2");
    printf("---> Entered product code: %d",shared_data.productCode);
    printf("---> Entered product description: %s",shared_data.price);
    printf("---> Entered product price: %.2f", shared_data.description);
  }
/* Disconnectsfor the data struct size*/
shared_data = munmap(addr, data_size);
  /* Check error */
  if (shared_data < 0) exit(1);
/* removes */
shared_data = shm_unlink("/share");
  /* Check error*/
  if (shared_data < 0) exit(1);
/* all descriptors are closed... */
exit(0);

}
