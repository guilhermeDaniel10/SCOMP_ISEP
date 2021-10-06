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
    sem_t *sem_writter, *sem_reader, *sem_isWriting;


    if ((sem_writter = sem_open("sem_writter", 0)) == SEM_FAILED)
    {
        perror("Error in sem_open_client()");
        exit(1);
    }

    if ((sem_reader = sem_open("sem_reader", 0)) == SEM_FAILED)
    {
        perror("Error in sem_open_client()");
        exit(1);
    }

    if ((sem_isWriting = sem_open("sem_isWriting", 0)) == SEM_FAILED)
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

    shared->qtdWriter++;

    sem_post(sem_writter); //------
    int value;
    sem_getvalue(sem_writter, &value);

    if (value > 1)
    {
        sem_wait(sem_isWriting);
    }

    time_t rawtime;
    struct tm *timeinfo;

    srand(time(NULL));
    int timeWaited = (rand() % (10 - 1 + 1)) + 1;
    sleep(timeWaited);

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    char str[100];
    sprintf(str, "My pid is %d and the local time is %s", getpid(), asctime(timeinfo));

    int currentIndex = shared->index;
    shared->index++;

    strcpy(shared->str[currentIndex], str);

    printf("Numero de writters atuais: %d\nNumero de readers atuais: %d\n", shared->qtdWriter, shared->qtdReader);

    shared->qtdWriter--;
    sem_post(sem_isWriting);

    return 0;
}