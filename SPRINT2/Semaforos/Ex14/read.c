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

typedef struct
{
    char str[10][100];
    int qtdReader;
    int qtdWriter;
    int index;
} info;

int main()
{

    sem_t *sem_writter, *sem_reader;

    if ((sem_writter = sem_open("sem_writter", 0)) == SEM_FAILED)
    {
        perror("Error in sem_open_client()");
        exit(1);
    }

    if ((sem_reader = sem_open("sem_reader", 1)) == SEM_FAILED)
    {
        perror("Error in sem_open_client()");
        exit(1);
    }

    int fd;

    int data_size = sizeof(info);

    fd = shm_open("/infoReader", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

    if (fd == -1)
    {
        perror("Erro a abrir a memoria partilhada.");
        exit(1);
    }

    ftruncate(fd, data_size);

    info *shared = (info *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    shared->qtdReader++;

    while(shared->qtdWriter != 0){};

    sem_wait(sem_writter);

    int currentIndex = shared->index;

    int i = 0;
    for (i = 0; i < currentIndex; i++)
    {
        printf("String: %s\n", shared->str[i]);
    }
    printf("Number of readers: %d\n", shared->qtdReader);

    shared->qtdReader--;
    return 0;
}