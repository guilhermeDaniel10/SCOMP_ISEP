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

#define NMR_PROCESSOS 1
#define MAX_C_and_S 5

typedef struct {
    //int index;
    int contS;
    int contC;
    int twoSfollowed;
    int twoCfollowed;
} maxSandC;

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

int main() {

    int fd;

    int data_size = sizeof(maxSandC);

    fd = shm_open("/ex08Sem", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);

    if (fd == -1) {
        perror("Erro a abrir a memoria partilhada.");
        exit(1);
    }

    ftruncate(fd, data_size);

    /*Obtem o pointer da estrutura da memoria partilhada */
    maxSandC *sharedCount = (maxSandC *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    sem_t *sem, *sem2, *sem3;

    if ((sem = sem_open("sem08", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED) {
        perror("Error in sem_open() 1");
        exit(1);
    }

    if ((sem2 = sem_open("sem08_2", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
        perror("Error in sem_open() 2");
        exit(1);
    }

    if ((sem3 = sem_open("sem08_3", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED) {
        perror("Error in sem_open() 3");
        exit(1);
    }

    //sharedCount->index = 0;
    sharedCount->contC = 0;
    sharedCount->contS = 0;
    sharedCount->twoSfollowed = 0;
    sharedCount->twoCfollowed = 0;
    //char *str = malloc(sizeof(*str));
    char str[2];
    int id = spawn_childs(NMR_PROCESSOS);
do {
    if (id > 0) {
      if (sharedCount->contS < MAX_C_and_S) {

        sem_wait(sem);
        sem_wait(sem3);

        strcpy(str,"S");
        puts(str);
        //*(str+sharedCount->index) = 'S';

        //sharedCount->index++;
        sharedCount->contS++;
        sharedCount->twoSfollowed++;

        if (sharedCount->twoSfollowed % 2 == 0) {
          sem_post(sem3);
          sem_post(sem2);
        } else {
          if (sharedCount->contS == MAX_C_and_S) {
            sem_post(sem3);
            sem_post(sem2);
          } else {
            sem_post(sem3);
            sem_post(sem);
          }
        }

      } else {
        exit(1);
      }

    } else {
      if (sharedCount->contC < MAX_C_and_S) {

        sem_wait(sem2);
        sem_wait(sem3);

        strcpy(str,"C");
        puts(str);
        //*(str+sharedCount->index) = 'C';

        //sharedCount->index++;
        sharedCount->contC++;
        sharedCount->twoCfollowed++;
        if (sharedCount->twoCfollowed % 2 == 0) {
          sem_post(sem3);
          sem_post(sem);
        } else {
          if (sharedCount->contC == MAX_C_and_S) {
            sem_post(sem3);
            sem_post(sem);
          } else {
            sem_post(sem3);
            sem_post(sem2);
          }
        }
      }
    }
} while(sharedCount->contS < MAX_C_and_S || sharedCount->contC < MAX_C_and_S);

      //puts(str);

      /* disconnects */
      fd = munmap(sharedCount, data_size);
      /* Check error */
      if (fd < 0) exit(1);
      /* removes the shared parte*/
      fd = shm_unlink("/ex08Sem");

      shm_unlink("/sem.sem08");
      sem_unlink("/sem08");
      sem_close(sem);
      shm_unlink("/sem.sem08_2");
      sem_unlink("/sem08_2");
      sem_close(sem2);
      shm_unlink("/sem.sem08_3");
      sem_unlink("/sem08_3");
      sem_close(sem3);

    return 0;
}
