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

#define NMR_PROCESSOS 50

typedef struct
{
    int place;
    char str[50][80];
} frase;

int main()
{
    int i = 0, fd;

    int data_size = sizeof(frase);

    pid_t pid;
    int status;

    shm_unlink("/ex03Sem");

    fd = shm_open("/ex03Sem", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);

    if (fd == -1)
    {
        perror("Erro a abrir a memoria partilhada.");
        exit(EXIT_FAILURE);
    }

    ftruncate(fd, data_size);

    /*Obtem o pointer da estrutura da memoria partilhada */
    frase *shared = (frase *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    sem_t *sem;

    if ((sem = sem_open("sem03", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED)
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

            char s[60];
            sprintf(s, "Tenho o valor PID: %d", getpid());

            strcpy(shared->str[shared->place], s);
            shared->place++;

            srand(time(NULL));

            // sleep(1);

            sleep((rand() % (5 - 1 + 1)) + 1);
            printf("A andar\n");

            sem_post(sem);
            exit(1);
        }
    }

    while ((pid = wait(&status)) > 0)
        ;
    int j = 0;

    for (j = 0; j < NMR_PROCESSOS; j++)
    {
        printf("Frase: %s\n", shared->str[j]);
    }

    if (shm_unlink("/sem.sem03") < 0)
    {
        printf("Erro no unlink da memoria partilhada\n");
        exit(EXIT_FAILURE);
    }
    sem_unlink("/sem03");

    sem_close(sem);

    return 0;
}