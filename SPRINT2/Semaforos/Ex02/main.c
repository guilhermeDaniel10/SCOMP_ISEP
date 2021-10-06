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
  sem_t *sem1,*sem2,*sem3,*sem4,*sem5,*sem6,*sem7,*sem8;

  if ((sem1 = sem_open("SEMAPHORES1", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED) {
  perror("Error in sem_open1()");
  exit(1);
  }
  if ((sem2 = sem_open("SEMAPHORES2", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
  perror("Error in sem_open2()");
  exit(1);
  }
  if ((sem3 = sem_open("SEMAPHORES3", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
  perror("Error in sem_open3()");
  exit(1);
  }
  if ((sem4 = sem_open("SEMAPHORES4", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
  perror("Error in sem_open4()");
  exit(1);
  }
  if ((sem5 = sem_open("SEMAPHORES5", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
  perror("Error in sem_open5()");
  exit(1);
  }
  if ((sem6 = sem_open("SEMAPHORES6", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
  perror("Error in sem_open6()");
  exit(1);
  }
  if ((sem7 = sem_open("SEMAPHORES7", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
  perror("Error in sem_open7()");
  exit(1);
  }
  if ((sem8 = sem_open("SEMAPHORES8", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
  perror("Error in sem_open8()");
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
  char str[80];

  if (id > 0) { /* child */

    switch(id) {
        case 1:
              sem_wait(sem1);
              sprintf(str,"Son Number %d reading \n", id);
              puts(str);
              break;

        case 2:
              sem_wait(sem2);
              sprintf(str,"Son Number %d reading \n", id);
              puts(str);
              break;

        case 3:
              sem_wait(sem3);
              sprintf(str,"Son Number %d reading \n", id);
              puts(str);
              break;

        case 4:
              sem_wait(sem4);
              sprintf(str,"Son Number %d reading \n", id);
              puts(str);
              break;

        case 5:
              sem_wait(sem5);
              sprintf(str,"Son Number %d reading \n", id);
              puts(str);
              break;

        case 6:
              sem_wait(sem6);
              sprintf(str,"Son Number %d reading \n", id);
              puts(str);
              break;

        case 7:
              sem_wait(sem7);
              sprintf(str,"Son Number %d reading \n", id);
              puts(str);
              break;

        case 8:
              sem_wait(sem8);
              sprintf(str,"Son Number %d reading \n", id);
              puts(str);
              break;
    }

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

        switch(id) {
            case 1:
                  sem_post(sem2);
                  exit(0);
                  break;

            case 2:
                  sem_post(sem3);
                  exit(0);
                  break;

            case 3:
                  sem_post(sem4);
                  exit(0);
                  break;

            case 4:
                  sem_post(sem5);
                  exit(0);
                  break;

            case 5:
                  sem_post(sem6);
                  exit(0);
                  break;

            case 6:
                  sem_post(sem7);
                  exit(0);
                  break;

            case 7:
                  sem_post(sem8);
                  exit(0);
                  break;

            case 8:
                  sem_post(sem8);
                  exit(0);
                  break;
        }
  }
int status = 0;
pid_t wpid;
 while((wpid = (wait(&status)>0))){}; /* wait for child */

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

   shm_unlink("/sem.SEMAPHORES1");
   sem_unlink("/SEMAPHORES1");
   sem_close(sem1);
   shm_unlink("/sem.SEMAPHORES8");
   sem_unlink("/SEMAPHORES8");
   sem_close(sem8);
   shm_unlink("/sem.SEMAPHORES7");
   sem_unlink("/SEMAPHORES7");
   sem_close(sem7);
   shm_unlink("/sem.SEMAPHORES6");
   sem_unlink("/SEMAPHORES6");
   sem_close(sem6);
   shm_unlink("/sem.SEMAPHORES5");
   sem_unlink("/SEMAPHORES5");
   sem_close(sem5);
   shm_unlink("/sem.SEMAPHORES4");
   sem_unlink("/SEMAPHORES4");
   sem_close(sem4);
   shm_unlink("/sem.SEMAPHORES3");
   sem_unlink("/SEMAPHORES3");
   sem_close(sem3);
   shm_unlink("/sem.SEMAPHORES2");
   sem_unlink("/SEMAPHORES2");
   sem_close(sem2);
}
