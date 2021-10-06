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

int main()
{

    sem_unlink("sem_writter");
    sem_unlink("sem_reader");
    sem_unlink("sem_isWriting");

    /*cria semáforos */
    sem_t *sem_writter, *sem_reader, *sem_isWriting;
    if ((sem_writter = sem_open("sem_writter", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED ||
        (sem_reader = sem_open("sem_reader", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED ||
        (sem_isWriting = sem_open("sem_isWriting", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED)
    {
        perror("Erro a criar o semaforo.\n");
        exit(0);
    }

    return 0;
}