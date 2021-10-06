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
    int nmr_tickets_Sold;
    int souldOut;
    int endOfTheDay;
} ticketData;

int main()
{

    int fd;

    int data_size = sizeof(ticketData);

    fd = shm_open("/sellingData", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

    if (fd == -1)
    {
        perror("Erro a abrir a memoria partilhada.");
        exit(1);
    }

    ftruncate(fd, data_size);

    sem_t *sem_salesman;

    if ((sem_salesman = sem_open("sem_salesman", 0)) == SEM_FAILED)
    {
        perror("Error in sem_open_salesman()");
        exit(1);
    }

    ticketData *ticketInfo = (ticketData *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    puts("The day came to an end... :(");
    ticketInfo->endOfTheDay = 1;

    sem_post(sem_salesman);

    return 0;
}
