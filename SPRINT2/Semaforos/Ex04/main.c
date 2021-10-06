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
#include <errno.h>

#define NMR_PROCESSOS 10

typedef struct{
    int place;
    char str[50][80];
} frase;

int main(){
    int i = 0, fd, s;

    int data_size = sizeof(frase);

    pid_t pid;
    int status;

    shm_unlink("/ex04Sem");

    fd = shm_open("/ex04Sem", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);

    if (fd == -1){
        perror("Erro a abrir a memoria partilhada.");
        exit(EXIT_FAILURE);
    }

    ftruncate(fd, data_size);

    /*Obtem o pointer da estrutura da memoria partilhada */
    frase *shared = (frase *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    sem_t *sem, *sem2;

    if ((sem = sem_open("sem04", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED){
        perror("Error in sem_open()");
        exit(1);
    }

    if ((sem2 = sem_open("sem04_2", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED){
        perror("Error in sem_open()");
        exit(1);
    }

    for (i = 0; i < NMR_PROCESSOS; i++)  {
        pid = fork();

        if (pid == 0) {

          //sem_wait(sem);
          struct timespec ts;
          if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
              /* handle error */
              return -1;
          }

          srand(time(NULL));
          sleep((rand() % (5 - 1 + 1)) + 1);

          ts.tv_sec += 12;
          while ((s = sem_timedwait(sem2, &ts)) == -1 && errno == EINTR)
                         continue;       /* Restart if interrupted by handler */
          /* Check what happened */

            if (s == -1) {
                if (errno == ETIMEDOUT){
                    printf("Succeeded, and waited 12 seconds\n");
                }else{
                    perror("error");
                }
            } else {
                printf("It wasn't possible to wait 12 seconds!\n");
            }

            sem_wait(sem);
            char s[60];
            sprintf(s, "Tenho o valor PID: %d", getpid());

            strcpy(shared->str[shared->place], s);
            shared->place++;
            
            sem_post(sem2);
            sem_post(sem);
            exit(1);
        }
    }

    while ((pid = wait(&status)) > 0);
    int j = 0;

    for (j = 0; j < NMR_PROCESSOS; j++){
          printf("%dº Frase: %s\n", j+1,shared->str[j]);
    }

    char validation[1];
    do {
      printf("Would you like to remove a line from the shared memory area (y/n)?\n");
      scanf("%s",validation);

    } while(validation[0] != 'y' && validation[0] != 'n');

    char clean[60] = "\0";
    char line[2];
    if (validation[0] == 'y') {
        printf("which line would you like to remove\n");
        scanf("%s",line);
        strcpy(shared->str[atoi(line)-1], clean);
        shared->place--;
        printf("Parte 2\n");
        for (j = atoi(line)-1; j < NMR_PROCESSOS; j++){
              strcpy(shared->str[j], shared->str[j+1]);
        }

        for (j = 0; j < shared->place; j++){
          if (atoi(line)-1 == j) {
              printf("Alterada ------------> %dº Frase: %s\n", j+1,shared->str[j]);
          } else {
              printf("%dº Frase: %s\n", j+1,shared->str[j]);
          }
        }
    }



    if (shm_unlink("/sem.sem04") < 0){
        printf("Erro no unlink da memoria partilhada\n");
        exit(EXIT_FAILURE);
    }

    if (shm_unlink("/sem.sem04_2") < 0){
        printf("Erro no unlink da memoria partilhada 2\n");
        exit(EXIT_FAILURE);
    }

    sem_unlink("/sem04");

    sem_close(sem);

    return 0;
}
