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

int spawn_childs(int nbrChilds)
{
    pid_t pid;
    int i;
    for (i = 0; i < nbrChilds; i++)
    {
        pid = fork();
        if (pid < 0)
        {
            return -1;
        }
        else if (pid == 0)
        {
            return i + 1;
        }
    }
    return 0;
}

void createSemaforos(sem_t *sem[3])
{
    char sufix[20];
    int j = 0;
    int start = 0;

    for (j = 0; j < 3; j++)
    {
        sprintf(sufix, "sem7%d", j + 1);
        if (j == 0)
        {
            start = 1;
        }
        if ((sem[j] = sem_open(sufix, O_CREAT | O_EXCL, 0644, start)) == SEM_FAILED)
        {
            perror("Error in sem_open()");
            exit(EXIT_FAILURE);
        }
        start = 0;
    }
}

void clearSemaforo(sem_t *sem[3])
{
    int j = 0;
    for (j = 0; j < 3; j++)
    {
        char sufix[20];
        sprintf(sufix, "/sem7%d", j + 1);
        char scdSufix[20];
        sprintf(scdSufix, "/sem.sem7%d", j + 1);

        if (shm_unlink(scdSufix) < 0)
        {

            exit(EXIT_FAILURE);
        }
        sem_unlink(sufix);

        sem_close(sem[j]);
    }
}

int main()
{
    sem_t *sem[3];
    createSemaforos(sem);
    int id = spawn_childs(3);
    //pid_t pid;

    //int status;

    if (id == 1)
    {
        sem_wait(sem[0]);
        puts("Sistema");

        sem_post(sem[1]);

        sem_wait(sem[0]);

        puts("a");

        sem_post(sem[1]);
        exit(EXIT_SUCCESS);
    }

    if (id == 2)
    {

        sem_wait(sem[1]);

        puts("de");

        sem_post(sem[2]);
        sem_wait(sem[1]);

        puts("melhor");

        sem_post(sem[2]);
        exit(EXIT_SUCCESS);
    }
    if (id == 3)
    {

        sem_wait(sem[2]);

        puts("Computadores");

        sem_post(sem[0]);
        sem_wait(sem[2]);

        puts("disciplina");

        exit(EXIT_SUCCESS);
    }

    int k = 0;
    for (k = 0; k < 3; k++)
    {
        wait(NULL);
    }

    clearSemaforo(sem);

    return 0;
}