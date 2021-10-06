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

#define MAX_CHILDREN 2
#define MAX_ESCRITAS 30
#define NM_SEMAFOROS 2

typedef struct
{
    int num[10];
    int num_escritas;
    int currentIndex;
} num;

void cleanSemaforos(sem_t *sem[NM_SEMAFOROS])
{
    int j = 0;
    for (j = 0; j < NM_SEMAFOROS; j++)
    {
        char sufix[20];
        char sufix2[20];
        sprintf(sufix, "/sem.sem13PL4%d", j + 1);
        sprintf(sufix2, "/sem13PL4%d", j + 1);

        if (shm_unlink(sufix) < 0)
        {

            exit(EXIT_FAILURE);
        }
        sem_unlink(sufix2);
        sem_close(sem[j]);
    }
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

/*
    Criar semaforos
*/
void createSemaforos(sem_t *sem[NM_SEMAFOROS])
{
    int j = 0;
    char sufix[20];
    for (j = 0; j < NM_SEMAFOROS; j++)
    {
        sprintf(sufix, "sem13PL4%d", j + 1);
        if (j == 0)
        { // LEITURA
            if ((sem[j] = sem_open(sufix, O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED)
            {
                perror("Error in sem_open()");
                exit(EXIT_FAILURE);
            }
        }
        if (j == 1)
        {
            if ((sem[j] = sem_open(sufix, O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED)
            {
                perror("Error in sem_open()");
                exit(EXIT_FAILURE);
            }
        }
    }
}

int main()
{

    int fd, data_size = sizeof(num);
    num *shared;
    sem_t *sem[NM_SEMAFOROS];

    /*Abre a memória partilhada */
    fd = shm_open("/ex13PL4", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);

    if (fd == -1)
    {
        printf("Erro a abrir");
        exit(EXIT_FAILURE);
    }

    ftruncate(fd, data_size);

    /*Obtem o pointer da estrutura da memoria partilhada */
    shared = (num *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    memset(&shared->num, 0, sizeof(shared->num)); /*Preenche toda a struct a 0 */
    shared->num_escritas = 0;
    shared->currentIndex = 0;

    createSemaforos(sem);

    int id = spawn_childs(MAX_CHILDREN);

    do
    {
        if (id == 0)
        {
            int value;
            sem_getvalue(sem[0], &value);

            sem_wait(sem[0]);

            int i = 0;
            for (i = 0; i < 10; i++)
            {
                printf("PAI -----> Numero atual: %d\n", shared->num[i]);
            }
            sem_post(sem[1]);
        }
        else
        {
            int value;
            sem_getvalue(sem[1], &value);

            sem_wait(sem[1]);

            if (shared->num_escritas % 10 != 0 || shared->num_escritas == 0)
            {

                int index = shared->currentIndex;
                shared->num[index] = shared->num_escritas + 1;
                shared->currentIndex++;
                shared->num_escritas++;

                if (shared->num_escritas % 10 == 0)
                {

                    int j = 0;
                    for (j = 0; j < 10; j++)
                    {
                        printf("FILHO -----> Numero atual: %d\n", shared->num[j]);
                    }

                    sem_post(sem[0]);

                }
                else
                {
                    sem_getvalue(sem[1], &value);

                    sem_post(sem[1]);
                }
            }
            else if (shared->num_escritas % 10 == 0 && shared->num_escritas != 0)
            {
                int j = 0;
                for (j = 0; j < 10; j++)
                {
                    printf("FILHO -----> Numero atual: %d\n", shared->num[j]);
                }
                shared->currentIndex = 0;
                int index = shared->currentIndex;
                shared->num[index] = shared->num_escritas + 1;
                shared->currentIndex++;
                shared->num_escritas++;
                sem_post(sem[1]);
            }
        }
    } while (shared->num_escritas < 30);
    if (id != 0)
    {
        exit(EXIT_SUCCESS);
    }

    if (shm_unlink("/ex13PL4") < 0)
    {
        printf("Sem necessidade de fechar.\n");
        exit(EXIT_FAILURE);
    }

    if (munmap(shared, data_size) < 0)
    {
        printf("Erro no munmap\n");
        exit(EXIT_FAILURE);
    }

    if (close(fd) < 0)
    {
        printf("Erro no close\n");
        exit(EXIT_FAILURE);
    }

    cleanSemaforos(sem);

    return 0;
}