#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int nbrGenerator();

int main(void) {
    /*The child process starts the game
    with a credit of 20 euros*/
    int saldo = 20;
    /*Notification to write in the pipe 1
    if it can make another bet,
     or 0, if the child’s credit ended;*/
    int notification = 1;
    /*aposta que o filho faz*/
    int aposta;
    /*Pipe One*/
    int fd1[2];
    if (pipe(fd1) == -1){
        perror("Erro no Pipe 1");
        return 1;
    }
    /*Pipe Two*/
    int fd2[2];
    if (pipe(fd2) == -1){
        perror("Erro no Pipe 2");
        return 1;
    }
    /*Criação desejada dos filhos*/
    pid_t pid = fork();
    // Check for errors
    if (pid == -1) {
      perror("Fork failed!");
      return 1;
    }
      /*Processo Pai*/
      if (pid > 0) {
        srand(time(NULL));
          /* fecha a extremidade de um dos pipes */
          close(fd1[0]);
          /* fecha a extremidade oposta do outro pipes */
          close(fd2[1]);
          /*generates a random number between 1 and 5*/
          int rndNumber;
          while(saldo > 0) {
            printf("\n-------------------\n");
            sleep(1);
            rndNumber = nbrGenerator();
            printf("Random number = %d",rndNumber);
            if (saldo > 0) {
              notification = 1;
            } else {
              notification = 0;
            }
            /*envia se pode ou não apostar*/
            write(fd1[1], &notification, sizeof(notification));
            /*recebe a aposta do filho*/
            if (notification == 1) {
              read(fd2[0], &aposta, sizeof(aposta));
              printf("\nAposta feita = %d\n",aposta);
            }
            if (aposta == rndNumber) {
              saldo += 10;
            } else {
              saldo -= 5;
            }
            /*envia o saldo para o filho*/
            write(fd1[1], &saldo, sizeof(saldo));
          }
          /* fecha a extremidade que já não usa */
          close(fd1[1]);
          /* fecha a extremidade que já não usa */
          close(fd2[0]);
      }

      /*Processos Filho*/
      if (pid == 0) {
        srand(time(NULL) * getpid());
        /* fecha a extremidade de um dos pipes */
        close(fd1[1]);
        /* fecha a extremidade oposta do outro pipes */
        close(fd2[0]);
        while (notification != 0) {
          /*recebe se pode ou não apostar*/
          read(fd1[0], &notification, sizeof(notification));
          if (notification != 0) {
            /*aposta que o filho vai fazer*/
            aposta = nbrGenerator();
            /*envia a aposta para o pai*/
            write(fd2[1], &aposta, sizeof(aposta));
          }
          /*recebe o saldo enviado pelo pai*/
          read(fd1[0], &saldo, sizeof(saldo));
          printf("Saldo disponível = %d\n",saldo);
          fflush(stdout);
        }
        /* fecha a extremidade que já não usa */
        close(fd1[0]);
        /* fecha a extremidade que já não usa */
        close(fd2[1]);
        exit(1);
      }

    return 0;
}

/*Função que gera os numeros no intervalo pedido*/
int nbrGenerator(){
    int upper = 5;
    int lower = 0;
    /* Print 5 random numbers from 1 to 5 */
    int rndNumber = (int) (rand() % (upper - lower + 1)) + lower;
    return rndNumber;
}
