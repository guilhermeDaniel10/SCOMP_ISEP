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

#define LIMITE 5
#define NM_SEMAFOROS 2
#define SHOW 10
sem_t *sem[NM_SEMAFOROS];

void cleanSemaforos(sem_t *sem[NM_SEMAFOROS])
{
    int j = 0;
    for (j = 0; j < NM_SEMAFOROS; j++)
    {
        char sufix[20];
        char sufix2[20];
        sprintf(sufix, "/sem.sem15%d", j + 1);
        sprintf(sufix2, "/sem15%d", j + 1);

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
    Criar semaforos
*/
void createSemaforos(sem_t *sem[NM_SEMAFOROS])
{
    int j = 0;
    char sufix[20];
    for (j = 0; j < NM_SEMAFOROS; j++)
    {
        sprintf(sufix, "sem15%d", j + 1);
        if (j == 1)
        {
            if ((sem[j] = sem_open(sufix, O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED)
            {
                perror("Error in sem_open()");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            if ((sem[j] = sem_open(sufix, O_CREAT | O_EXCL, 0644, LIMITE)) == SEM_FAILED)
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
    createSemaforos(sem);

    // Os processos correspondem aos visitors do show room
    int id = spawn_childs(20);

    if (id != 0)
    {
        while (1)
        {
            // Todos os visitantes comecam por esperar a porta que seja possivel entrar, mesmo os primeiros
            printf("%d is at the door waiting\n", id);
            sem_wait(sem[0]);
            int isFull;
            sem_getvalue(sem[1], &isFull);

            // Um visitante so pode entrar se o limite nao tiver sido excedido
            if (isFull < LIMITE)
            {

                // O visitante entra e incrementa o numero de visitantes dentro da sala
                printf("%d just entered\n", id);
                sem_post(sem[1]);
      

                sleep(SHOW);

                // O visitante sai e decremente o numero de visitantes dentro da sala e o semaforo de organizacao de visitantes passa a 1 e quem esperava pode entrar
                printf("%d is leaving the show\n", id);
                sem_post(sem[0]);
                sem_wait(sem[1]);
                break;
            }
        }
        exit(EXIT_SUCCESS);
    }
    int i = 0;
    for (i = 0; i < 20; i++)
    {
        wait(NULL);
    }

    cleanSemaforos(sem);
    return 0;
}