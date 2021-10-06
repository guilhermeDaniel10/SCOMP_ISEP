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

#define NMR_TICKETS_TO_SELL 2

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

  ticketData *ticketInfo = (ticketData *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  ticketInfo->endOfTheDay = 0;

  ticketInfo->nmr_tickets_Sold = 0;
  ticketInfo->souldOut = 0;

  sem_t *sem_client, *sem_salesman;

  if ((sem_client = sem_open("sem_client", O_CREAT, 0644, 1)) == SEM_FAILED)
  {
    perror("Error in sem_open_client()");
    exit(1);
  }

  if ((sem_salesman = sem_open("sem_salesman", O_CREAT, 0644, 0)) == SEM_FAILED)
  {
    perror("Error in sem_open_salesman()");
    exit(1);
  }

  char strSalesman[100];

  do
  {
    puts("Waiting for clients...");
    sem_wait(sem_salesman);

    if (ticketInfo->endOfTheDay == 1)
    {
      break;
    }

    if (ticketInfo->souldOut == 1)
    {
      ticketInfo->souldOut = 1;
      strcpy(strSalesman, "Sorry, I'm out of tickets!\nCome again tomorrow!\n");
      puts(strSalesman);
      sem_post(sem_client);
    }

    if ((ticketInfo->nmr_tickets_Sold) <= NMR_TICKETS_TO_SELL && ticketInfo->souldOut != 1)
    {
      strcpy(strSalesman, "Yes, I still have tickets!\nIt will be 10€!\n");
      puts(strSalesman);

      if ((ticketInfo->nmr_tickets_Sold) + 1 > NMR_TICKETS_TO_SELL)
      {
        ticketInfo->souldOut = 1;
      }

      sem_post(sem_client);
    }

  } while (ticketInfo->endOfTheDay == 0);

  shm_unlink("/sellingData");

  shm_unlink("/sem.sem_client");
  sem_unlink("/sem_client");

  shm_unlink("/sem.sem_salesman");
  sem_unlink("/sem_salesman");
}
