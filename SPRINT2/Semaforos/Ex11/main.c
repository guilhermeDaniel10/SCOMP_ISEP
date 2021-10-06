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

#define NM_SEMAFOROS 4
#define LIMITE 200
sem_t *sem[NM_SEMAFOROS];

//Terminar com CNTL-C

/*
Eliminar todos os semaforos
*/
void cleanSemaforos(sem_t *sem[NM_SEMAFOROS])
{
    int j = 0;
    for (j = 0; j < NM_SEMAFOROS; j++)
    {
        char sufix[20];
        char sufix2[20];
        sprintf(sufix, "/sem.sem11%d", j + 1);
        sprintf(sufix2, "/sem11%d", j + 1);

        if (shm_unlink(sufix) < 0)
        {

            exit(EXIT_FAILURE);
        }
        sem_unlink(sufix2);
        sem_close(sem[j]);
    }
}

/*
    Handler do control C
*/
void handler(int signo)
{
    int k = 0;
    if (signo == SIGINT)
    {
        for (k = 0; k < 3; k++)
        {
            wait(NULL);
        }
        cleanSemaforos(sem);
        exit(0);
    }
}
/*
    Cria n processos
*/
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
        sprintf(sufix, "sem11%d", j + 1);
        if (j == 3)
        {
            if ((sem[j] = sem_open(sufix, O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED)
            {
                perror("Error in sem_open()");
                exit(EXIT_FAILURE);
            }
        }
        else
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

    /*
        Cria um tratamento de sinal CNTL-C para terminar o programa e acabar com todos os processos a executar,
        assim como todos os semaforos ativos
    */
    struct sigaction act;
    memset(&act, 0, sizeof(struct sigaction)); /*Preenche toda a struct a 0 */
    sigemptyset(&act.sa_mask);
    act.sa_handler = handler;
    sigaction(SIGINT, &act, NULL); /*Substituir aqui o sinal a ser lido */

    // Cria semaforos
    createSemaforos(sem);

    // Cria tres processos
    int id = spawn_childs(3);

    // Variavel que vai ter o valor de cada semaforo correspondente a capacidade do comboio
    int value;

    if (id == 1)
    {

        while (1)
        {
            if (value == 256)
            {
            }
            else
            {
                printf("Capacidade atual: %d\n", value);
            }

            /*
                Randomiza uma entrada ou saida
            */
            srand(time(NULL));
            int choice = (rand() % 2) + 1;

            // Obtem o valor do semafoto
            sem_getvalue(sem[3], &value);
            printf("TERMINAR COM CNTL-C\n");

            if (choice == 1)
            {
                // Decrementa o semaforo que coordena a primeira porta
                sem_wait(sem[0]);

                // Determina se e possivel decrementar o semaforo, se nao for, ninguem consegue sair,
                // se nao, sai
                if (sem_trywait(sem[3]) < 0)
                {
                    printf("Ninguem saiu pela 1a porta.\n");
                }
                else
                {
                    printf("A sair pela 1a porta.\n");
                }

                // Incrementa o semaforo que coordena a primeira porta
                sem_post(sem[0]);
                sleep(4);
            }
            else
            {

                sem_wait(sem[0]);
                if (value < LIMITE)
                {
                    sem_post(sem[0]);
                    sem_post(sem[3]);
                    printf("Entrou um passeiro pela 1a porta.\n");
                }
                else
                {
                    sem_post(sem[3]);
                    printf("Ninguem saiu da 1a porta.\n");
                }
                sleep(4);
            }
        }

        exit(EXIT_SUCCESS);
    }
    if (id == 2)
    {
        while (1)
        {
            if (value == 256)
            {
            }
            else
            {
                printf("Capacidade atual: %d\n", value);
            }

            srand(time(NULL));
            int choice = (rand() % 2) + 1;

            sem_getvalue(sem[3], &value);
            printf("TERMINAR COM CNTL-C\n");

            if (choice == 1)
            {
                sem_wait(sem[1]);

                if (sem_trywait(sem[3]) < 0)
                {
                    printf("Ninguem saiu pela 1a porta.\n");
                }
                else
                {
                    printf("A sair pela 1a porta.\n");
                }

                sem_post(sem[1]);

                sleep(4);
            }
            else
            {
                sem_wait(sem[1]);
                if (value < LIMITE)
                {
                    sem_post(sem[3]);
                    sem_post(sem[1]);
                    printf("Entrou um passeiro pela 2a porta.\n");
                }
                else
                {
                    sem_post(sem[1]);
                    printf("Ninguem saiu da 2a porta.\n");
                }
                sleep(4);
            }
        }

        exit(EXIT_SUCCESS);
    }
    if (id == 3)
    {
        while (1)
        {
            if (value == 256)
            {
            }
            else
            {
                printf("Capacidade atual: %d\n", value);
            }

            srand(time(NULL));
            int choice = (rand() % 2) + 1;

            sem_getvalue(sem[3], &value);
            printf("TERMINAR COM CNTL-C\n");

            if (choice == 1)
            {
                sem_wait(sem[2]);

                if (sem_trywait(sem[3]) < 0)
                {
                    printf("Ninguem saiu pela 1a porta.\n");
                }
                else
                {
                    printf("A sair pela 1a porta.\n");
                }

                sem_post(sem[2]);

                sleep(4);
            }
            else
            {
                sem_wait(sem[2]);
                if (value < LIMITE)
                {
                    sem_post(sem[2]);
                    sem_post(sem[3]);
                    printf("Entrou um passeiro pela 3a porta.\n");
                }
                else
                {
                    sem_post(sem[2]);
                    printf("Ninguem saiu da 3a porta.\n");
                }
                sleep(4);
            }
        }

        exit(EXIT_SUCCESS);
    }

    int k = 0;
    for (k = 0; k < 3; k++)
    {
        wait(NULL);
    }

    cleanSemaforos(sem);

    return 0;
}