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

#define MAX_CHILDS 400
#define MAX_CAPACITY 300

typedef struct
{
    int qtdVIP;
    int qtdSpecial;
    int qtdNormal;
    int fullCapacityOnce;
} fullCapOnde;

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

char *statusPessoa(int n)
{
    if (n % 5 == 0)
    {
        return "VIP";
    }
    else if (n % 3 == 0 && n % 5 != 0)
    {
        return "SPECIAL";
    }
    return "NORMAL";
}

int statusAsInt(int n)
{
    if (n % 5 == 0)
    {
        return 1;
    }
    else if (n % 3 == 0 && n % 5 != 0)
    {
        return 2;
    }
    return 3;
}

int main()
{
    int status;
    pid_t pid;

    int fd;

    int data_size = sizeof(fullCapOnde);

    fd = shm_open("/ex17SH", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);

    if (fd == -1)
    {
        perror("Erro a abrir a memoria partilhada.");
        exit(1);
    }

    ftruncate(fd, data_size);

    /*Obtem o pointer da estrutura da memoria partilhada */
    fullCapOnde *sharedCount = (fullCapOnde *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    sharedCount->fullCapacityOnce = 0;
    sharedCount->qtdVIP = 0;
    sharedCount->qtdSpecial = 0;
    sharedCount->qtdNormal = 0;

    sem_t *semEntrada, *semVisitante, *semCapacidade, *semCount, *semSpecial, *semNormal, *semLine;

    if ((semEntrada = sem_open("semEntrada", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED)
    {
        perror("Error in sem_open()");
        exit(EXIT_FAILURE);
    }

    if ((semVisitante = sem_open("semVisitante", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED)
    {
        perror("Error in sem_open()");
        exit(EXIT_FAILURE);
    }

    if ((semCapacidade = sem_open("semCapacidade", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED)
    {
        perror("Error in sem_open()");
        exit(EXIT_FAILURE);
    }

    if ((semCount = sem_open("semCount", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED)
    {
        perror("Error in sem_open()");
        exit(EXIT_FAILURE);
    }

    if ((semSpecial = sem_open("semSpecial", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED)
    {
        perror("Error in sem_open()");
        exit(EXIT_FAILURE);
    }

    if ((semNormal = sem_open("semNormal", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED)
    {
        perror("Error in sem_open()");
        exit(EXIT_FAILURE);
    }

    if ((semLine = sem_open("semLine", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED)
    {
        perror("Error in sem_open()");
        exit(EXIT_FAILURE);
    }

    int id = spawn_childs(MAX_CHILDS);

    if (id != 0)
    {
        sem_wait(semCount);
  
        int currentStat = statusAsInt(id);

        if (currentStat == 1)
        {
            sharedCount->qtdVIP++;
        }
        else if (currentStat == 2)
        {
            sharedCount->qtdSpecial++;
        }
        else if (currentStat == 3)
        {
            sharedCount->qtdNormal++;
        }

        sem_post(semCount);

        if ((sharedCount->qtdSpecial) + (sharedCount->qtdVIP) + (sharedCount->qtdNormal) != MAX_CHILDS)
        {
            sem_wait(semLine);
            sem_post(semLine);
        }
        else
        {
            sem_post(semLine);
        }
        sem_wait(semEntrada);

        if (sharedCount->fullCapacityOnce == 1)
        {
            if (currentStat == 1)
            {
                sharedCount->qtdVIP--;
            }
            else if (currentStat == 2)
            {
                if (sharedCount->qtdVIP > 0)
                {
                    sem_wait(semSpecial);
                    sem_post(semSpecial);
                }
                sharedCount->qtdSpecial--;
            }
            else if (currentStat == 3)
            {
                if (sharedCount->qtdVIP > 0 && sharedCount->qtdSpecial > 0)
                {
                    sem_wait(semNormal);
                    sem_post(semNormal);
                }
                sharedCount->qtdNormal--;
            }
        }
        else
        {
            if (currentStat == 1)
            {
                sharedCount->qtdVIP--;
            }
            else if (currentStat == 2)
            {
                sharedCount->qtdSpecial--;
            }
            else if (currentStat == 3)
            {
                sharedCount->qtdNormal--;
            }
        }

        sem_post(semVisitante);

        int value;
        sem_getvalue(semVisitante, &value);

        //Para quando atingir a capacidade maxima

        if (value <= MAX_CAPACITY && sharedCount->fullCapacityOnce == 0)
        {
            printf("Entrou pessoa com status de %s e com id %d\n", statusPessoa(id), id);
            sem_post(semEntrada);
        }

        if (sharedCount->fullCapacityOnce == 1)
        {
            printf("Entrou depois da capacidade maxima uma pessoa com status de %s e com id %d\n", statusPessoa(id), id);
           
            sem_post(semEntrada);
        }

        if (value == MAX_CAPACITY)
        {
            puts("CAPACIDADE MAXIMA ATINGIDA");
            sharedCount->fullCapacityOnce = 1;
            sem_post(semCapacidade);
        }

        sem_wait(semCapacidade);
        sem_post(semCapacidade);

        /*srand(time(NULL));
        int timeWaited = (rand() % (4 - 1 + 1)) + 1;
        sleep(timeWaited);*/

        //printf("Saiu uma pessoa com status de %s e com id %d\n", statusPessoa(id), id);

        sem_wait(semVisitante);

        if (sharedCount->fullCapacityOnce == 1)
        {

 
            if (sharedCount->qtdVIP == 0 && sharedCount->qtdSpecial > 0)
            {

                sem_post(semSpecial);
            }
            if (sharedCount->qtdVIP == 0 && sharedCount->qtdSpecial == 0 && sharedCount->qtdNormal > 0)
            {

                sem_post(semNormal);
            }
            sem_post(semEntrada);
        }
        int value2;
        sem_getvalue(semEntrada, &value2);

        if (value2 == 0)
        {
            sem_post(semEntrada);
        }

        exit(EXIT_SUCCESS);
    }

    while ((pid = wait(&status)) > 0)
        ;

    if (shm_unlink("/sem.semEntrada") < 0)
    {
        printf("ERRO UNLINK Entrada\n");
        exit(EXIT_FAILURE);
    }
    sem_unlink("/sem.semEntrada");
    sem_close(semEntrada);
    //---------------------------------------------
    if (shm_unlink("/sem.semVisitante") < 0)
    {
        printf("ERRO UNLINK VISITANTE\n");
        exit(EXIT_FAILURE);
    }
    sem_unlink("/sem.semVisitante");
    sem_close(semVisitante);
    //---------------------------------------------
    if (shm_unlink("/sem.semCapacidade") < 0)
    {
        printf("ERRO UNLINK CAPACIDADE\n");
        exit(EXIT_FAILURE);
    }
    sem_unlink("/sem.semCapacidade");
    sem_close(semCapacidade);
    //---------------------------------------------

    if (shm_unlink("/sem.semCount") < 0)
    {
        printf("ERRO UNLINK COUNT\n");
        exit(EXIT_FAILURE);
    }
    sem_unlink("/sem.semCount");
    sem_close(semCount);
    //---------------------------------------------

    if (shm_unlink("/sem.semSpecial") < 0)
    {
        printf("ERRO UNLINK COUNT\n");
        exit(EXIT_FAILURE);
    }
    sem_unlink("/sem.semSpecial");
    sem_close(semCount);
    //---------------------------------------------

    if (shm_unlink("/sem.semNormal") < 0)
    {
        printf("ERRO UNLINK COUNT\n");
        exit(EXIT_FAILURE);
    }
    sem_unlink("/sem.semNormal");
    sem_close(semCount);

    //---------------------------------------------

    if (shm_unlink("/sem.semLine") < 0)
    {
        printf("ERRO UNLINK COUNT\n");
        exit(EXIT_FAILURE);
    }
    sem_unlink("/sem.semLine");
    sem_close(semCount);

    /* disconnects */
    fd = munmap(sharedCount, data_size);
    /* Check error */
    if (fd < 0)
        exit(1);
    /* removes the shared parte*/
    fd = shm_unlink("/ex17SH");

    return 0;
}