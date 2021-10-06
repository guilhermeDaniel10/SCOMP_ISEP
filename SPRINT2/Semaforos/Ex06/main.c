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

#define NMR_PROCESSOS 15

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
    sem_t *sem, *sem2;

    if ((sem = sem_open("sem06", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED)
    {
        perror("Error in sem_open()");
        exit(1);
    }

    if ((sem2 = sem_open("sem062", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED)
    {
        perror("Error in sem_open()");
        exit(1);
    }

    int id = spawn_childs(NMR_PROCESSOS);
    if (id > 0) {
      sem_wait(sem2);

      printf("I'm the child with PID %d\n", getpid());

      sem_post(sem);
      exit(1);
    } else {
      int status = 0, incr = 1;
      pid_t wpid;
      while((wpid = (wait(&status)>0))){
        sem_wait(sem);

        printf("I'm the father with PID %d\n", getpid());
        printf("-------------%d-----------\n", incr);
        incr++;
        sem_post(sem2);
      } /* wait for child */

    }


    if (shm_unlink("/sem.sem06") < 0){
        printf("Erro no unlink da memoria partilhada\n");
        exit(EXIT_FAILURE);
    }
    sem_unlink("/sem06");

    sem_close(sem);

    if (shm_unlink("/sem.sem062") < 0){
        printf("Erro no unlink da memoria partilhada\n");
        exit(EXIT_FAILURE);
    }
    sem_unlink("/sem062");

    sem_close(sem2);

    return 0;
}
