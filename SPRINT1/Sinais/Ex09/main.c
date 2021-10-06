#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <time.h>


int timeCnt = 1;

/*A variavel volatil vai indicar se o sinal foi retornado ou nao */
volatile sig_atomic_t esperar = 0;

/*Handler do alarme A */
void handle_alarmA(int signo){
  char str[120];
  sprintf(str, "Passaram %d segundos\n", timeCnt);
  write(STDOUT_FILENO, str, strlen(str));
}

/* Handler do alarme B */
void handle_alarmB(int signo){
  char *str = "TaskB\n";
  write(STDOUT_FILENO, str, strlen(str));
}

/* Handler do sinal sigusr1 */
void handle_SIGUSR1(int signo, siginfo_t *sinfo, void *context){
  char *str = "TaskC\n";
  write(STDOUT_FILENO, str, strlen(str));
  esperar = 1; /*Variavel que indica que o sinal foi executado */
}


int main(){

  pid_t pid = fork();

  /*processo filho */
  if(pid == 0){

    /*Gerar um número de 1 a 5 */
    time_t t;
    srand ((unsigned) time (&t));
    int num = rand() % 5 + 1;

    /*Rececao do sinal do alarme com o numero gerado*/
    struct sigaction act;
    memset(&act, 0, sizeof(struct sigaction));
    sigemptyset(&act.sa_mask);
    act.sa_handler = handle_alarmB;
    sigaction(SIGALRM, &act, NULL);
    alarm(num);



    memset(&act, 0, sizeof(struct sigaction));
    sigemptyset(&act.sa_mask);
    act.sa_sigaction = handle_SIGUSR1;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &act, NULL);

    esperar = 0;
    /*Espera que o pai envie o sinal */
    while(!esperar);

  } /*processo pai */
  else if(pid > 0){

      /*Rececao do sinal do alarme */
      struct sigaction act;
      memset(&act, 0, sizeof(struct sigaction));
      sigemptyset(&act.sa_mask);
      act.sa_handler = handle_alarmA;
      sigaction(SIGALRM, &act, NULL);

      int i = 0;
      sleep(2);/*A unica maneira de garantir que todos os processos eram executados
      foi fazer este sleep */

      /*Envia um alarme 3 vezes para imprimir os segundos */
      for(i = 0; i < 3; i++){
          alarm(1);
          sleep(1);
          timeCnt++;
      }
      /*Envia o sinal SIGUSR1 */
      kill(pid, SIGUSR1);


  } else {
    perror("ERRO");
  }

}
