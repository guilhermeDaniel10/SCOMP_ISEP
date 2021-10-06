#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>

#define ARR_SIZE 200
#define NBR_CHILDREN 8

int spawn_childs(int nbrChilds){
    pid_t pid;
    int i;
    for (i = 0; i < nbrChilds; i++){
        pid = fork();
        if (pid < 0) {
          return -1;
        } else if (pid == 0) {
          return i + 1;
        }
    }
    return 0;
}

int main(void) {
  /*
  File descriptor.
  */
 int fd;
   /*
   Size of the struct being shared.
   */
   int data_size = sizeof(int);
   /*
   Allows to request the area to be mapped in a specific address.
   */
   int *nbrChildren;
   /*
   Creates (O_CREAT) shared memory area (with error if existis because of O_EXCL)
   with nome “/share”, and read/write permissons (O_RDWR), and open for user to
   read (S_IRUSR) and write (S_IWUSR).
   */
   fd = shm_open("/shared_Counter", O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR);
   /*
   Defines the size of the area(struct aluno) and initializes it to zero
   */
   ftruncate (fd, data_size);
   /*
   Shares the mapped area to the pointer
   */
   nbrChildren = (int *)mmap(NULL,data_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

  /* shared mem */
  sem_t *sem;
  /* create, open and map shared memory to shm_counter */
  /* create semapore with value = 1 */
  if ((sem = sem_open("SEMAPHORES9", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED) {
  perror("Error in sem_open()");
  exit(1);
  }
  *nbrChildren = 0; /* shared memory init */
  FILE *filePointer,*filePointer2;

  filePointer2 = fopen("Output.txt","w");
  if(filePointer2 == NULL) {
    /* File not created hence exit */
    printf("Unable to create file.\n");
    exit(EXIT_FAILURE);
  }else{
    fclose(filePointer2);
  }
  int id = spawn_childs(NBR_CHILDREN);

  if (id > 0) { /* child */
      sem_wait(sem);
      filePointer = fopen("Numbers.txt", "r");
      if (filePointer != NULL) {
        filePointer2 = fopen("Output.txt","a");
        (*nbrChildren)++;
        int maxNbrLines = 200*(*nbrChildren);
        int numberOfLines = 0;
        int numero;
          while(numberOfLines < maxNbrLines) {
            fscanf(filePointer, "%d", &numero);
            if (200*((*nbrChildren)-1) <= numberOfLines) {
              fprintf(filePointer2, "%d\n", numero);
            }
            numberOfLines++;
          }

         fclose(filePointer);
         fclose(filePointer2);
       }

        /* disconnects */
        fd = munmap(nbrChildren, data_size);
        /* Check error */
        if (fd < 0) exit(1);
        /* removes the shared parte*/
        fd = shm_unlink("/shared_Counter");
      sem_post(sem);
      exit(0);
  }
 wait(NULL); /* wait for child */

   fd = munmap(nbrChildren, data_size);
   /* Check error */
   if (fd < 0) exit(1);
   /* closes pipe */
   fd = close(fd);
   int numero = 0;
   filePointer2 = fopen("Output.txt","r");

   do{
       fscanf(filePointer2, "%d", &numero);
       printf("%d\n",numero);
   }while(numero != NBR_CHILDREN*ARR_SIZE);

   shm_unlink("/sem.SEMAPHORES9");
   sem_unlink("/SEMAPHORES9");
   sem_close(sem);
}
