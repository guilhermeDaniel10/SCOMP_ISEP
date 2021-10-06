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
} ticketData;

int main()
{
    puts("Waiting in line...");
    int fd;

    int data_size = sizeof(ticketData);

    fd = shm_open("/sellingData", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

    if (fd == -1)
    {
        perror("Erro a abrir a memoria partilhada.");
        exit(1);
    }

    ftruncate(fd, data_size);

    ticketData *ticketInfo = (ticketData *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    sem_t *sem_client, *sem_salesman;

    if ((sem_client = sem_open("sem_client", 0)) == SEM_FAILED)
    {
        perror("Error in sem_open_client()");
        exit(1);
    }

    if ((sem_salesman = sem_open("sem_salesman", 0)) == SEM_FAILED)
    {
        perror("Error in sem_open_salesman()");
        exit(1);
    }

    char str[100];

    char strTimeWaited[100];

    sem_wait(sem_client);

    srand(time(NULL));
    int timeWaited = (rand() % (10 - 1 + 1)) + 1;
    sleep(timeWaited);
    sprintf(strTimeWaited, "I have waited %d hours in line for a ticket!\n", timeWaited);
    puts(strTimeWaited);

    if (ticketInfo->souldOut != 1)
    {

        ticketInfo->nmr_tickets_Sold++;
        sprintf(str, "YES! I got the ticket number %d!\n", ticketInfo->nmr_tickets_Sold);
        puts(str);
        
        sem_post(sem_salesman);
    }
    else
    {
        strcpy(str, "Damn it, I really wanted a ticket!");
        puts(str);
        sem_post(sem_salesman);
    }
}
