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

#define NMR_PROCESSOS 10

int main()
{

    pid_t pid;
    int i = 0;

    sem_t *sem, *sem2;

    shm_unlink("/ex05Sem");
    shm_unlink("/ex052Sem");

    if ((sem = sem_open("sem05", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED)
    {
        perror("Error in sem_open()");
        exit(1);
    }

    if ((sem2 = sem_open("sem052", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED)
    {
        perror("Error in sem_open()");
        exit(1);
    }

    for (i = 0; i < NMR_PROCESSOS; i++)
    {
        pid = fork();

        if (pid == 0)
        {
            sem_wait(sem);

            printf("I'm the child with PID %d\n", getpid());

            sem_post(sem2);
            exit(1);
        }
        else
        {
            sem_wait(sem2);

            printf("I'm the father with PID %d\n\n", getpid());

            sem_post(sem);
        }
    }

    if (shm_unlink("/sem.sem05") < 0)
    {
        printf("Erro no unlink da memoria partilhada\n");
        exit(EXIT_FAILURE);
    }
    sem_unlink("/sem05");

    sem_close(sem);

    if (shm_unlink("/sem.sem052") < 0)
    {
        printf("Erro no unlink da memoria partilhada\n");
        exit(EXIT_FAILURE);
    }
    sem_unlink("/sem052");

    sem_close(sem2);

    return 0;
}