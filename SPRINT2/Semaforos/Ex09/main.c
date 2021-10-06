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

void cleanSemaforo(sem_t *sem[2])
{

    if (shm_unlink("/sem.sem91") < 0)
    {

        exit(EXIT_FAILURE);
    }
    if (shm_unlink("/sem.sem92") < 0)
    {

        exit(EXIT_FAILURE);
    }

    sem_unlink("/sem91");
    sem_unlink("/sem92");

    sem_close(sem[0]);
    sem_close(sem[1]);
}

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

void createSemaforos(sem_t *sem[2])
{

    if ((sem[0] = sem_open("sem91", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED)
    {
        perror("Error in sem_open()");
        exit(EXIT_FAILURE);
    }
    if ((sem[1] = sem_open("sem92", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED)
    {
        perror("Error in sem_open()");
        exit(EXIT_FAILURE);
    }
}

int main()
{
    sem_t *sem[2];

    createSemaforos(sem);
    int id = spawn_childs(2);

    if (id == 1)
    {

        puts("buy chips");

        sem_post(sem[0]);
        sem_post(sem[0]);

        if (sem_wait(sem[0]) > -1 && sem_wait(sem[1]) > -1)
        { //sucesso
            puts("eat and drink");
        }

        exit(EXIT_SUCCESS);
    }
    if (id == 2)
    {

        puts("buy beer");
        sem_post(sem[1]);
        sem_post(sem[1]);

        if (sem_wait(sem[0]) > -1 && sem_wait(sem[1]) > -1)
        { //sucesso
            puts("eat and drink");
        }

        exit(EXIT_SUCCESS);
    }

    int j = 0;
    for (j = 0; j < 2; j++)
    {
        wait(NULL);
    }

    cleanSemaforo(sem);
    return 0;
}