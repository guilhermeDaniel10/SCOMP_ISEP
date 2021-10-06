#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

int qtdFilhos = 5;

int sigaction(int sig,
const struct sigaction * act,
struct sigaction * oact);

void handle_USR1(int signo, siginfo_t *sinfo, void *context) {
  qtdFilhos--;
  /*Alternativa mais segura ao printf*/
  char *str = "\n\n------------------Um filho terminou!-----------------\n\n";
  write(STDOUT_FILENO,str,strlen(str));
}

void print(int max, int min, int processo);

int main() {

  /* sigaction struct. */
  struct sigaction act;

  /* Clear the act variable. */
  memset(&act, 0, sizeof(struct sigaction));

   /* All signals blocked */
   sigfillset(&act.sa_mask);

  /* Receive and handle SIGUSR1 */
  act.sa_sigaction = handle_USR1;
  act.sa_flags = SA_SIGINFO;
  sigaction(SIGUSR1, &act, NULL);

  int p;
  int pidFilhos[5];

  for (int i = 0; i < 5; i++) {
    p = fork();
    if (p == 0) {
      int parentPID = getppid();
      print((i+5)*200,i*200,i+1);
      kill(parentPID,10);
      exit(i+1);
    } else {
      pidFilhos[i] = p;
    }
  }

  while(qtdFilhos!=0){
    pause();
  }

  wait(pidFilhos);

}

void print(int max, int min, int processo){
  int upper = max;
  int lower = min;

  for(int i = lower; i <= upper; i++){
      printf("Processo %d = %d\n",processo,i);
  }
}
