#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

typedef struct {
 char message[80];
 int rnd;
} game;

#define NUMBER_OF_CHILDREN 10
#define NUMBER_OF_ROUNDS 10

/*Exercício 12 processos*/
int spawn_childs(int nbrChilds){
    pid_t pid;
    int i;
    for (i = 0; i < nbrChilds; i++){
        pid = fork();
        if (pid < 0) {
          return -1;
        } else if (pid == 0) {
          return i + 1;
        }
    }
    return 0;
}

int main(void) {
    /*Pipe*/
    int fd[2];

    if (pipe(fd) == -1){
        perror("Erro no Pipe");
        return 1;
    }
    /*Criação desejada dos filhos*/
    int id = spawn_childs(NUMBER_OF_CHILDREN);

    /*Processo Pai*/
    if (id == 0) {
        /* fecha a extremidade não usada pai só escreve */
        close(fd[0]);
        int i;
        /*estrutura do jogo*/
        game gameOfPipe;

        for (i = 0; i < NUMBER_OF_CHILDREN; i++){
          /*Inicializar estrutura*/
          strcpy(gameOfPipe.message, "WIN");
          gameOfPipe.rnd = i + 1;
          /*enviar ronda*/
          write(fd[1], &gameOfPipe, sizeof(gameOfPipe));
          /*esperar por nova ronda*/
          sleep(2);
        }
        /* fecha a extremidade de escrita por já não ser mais usada */
        close(fd[1]);
        printf("\n------------------------------------\n\n");
    }

    /*Processos Filho*/
    if (id > 0) {
      /* fecha a extremidade não usada, filho só lê */
      close(fd[1]);
      /* Informação do jogo */
      game gameInfo;
      /*lê ronda*/
      read(fd[0], &gameInfo, sizeof(gameInfo));

      close(fd[0]);
        printf("Filho %d venceu a ronda %d\n", id, gameInfo.rnd);
        exit(gameInfo.rnd);
    }

    int status;
    int j;
    /*Espera que todos os filhos tenham terminado*/
    for (j = 0; j < NUMBER_OF_CHILDREN; j++) {
        pid_t pid = wait(&status);
        if (WIFEXITED(status)) {
            /*Valida para cada filho qual a ronda em que venceu*/
            int winningRnd = WEXITSTATUS(status);
            printf("Filho com o PID %d venceu a ronda %d\n", pid, winningRnd);
        }
    }
    return 0;
}
