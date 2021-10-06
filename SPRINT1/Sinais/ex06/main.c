#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int counter = 0;

int sigaction(int sig,
const struct sigaction * act,
struct sigaction * oact);

void handle_USR1(int signo, siginfo_t *sinfo, void *context) {
  counter++;
  /*Alternativa mais segura ao printf*/
  char str[80];
  sprintf(str, "SIGUSR1 signal captured, USR1_COUNTER = %d\n", counter);
  write(STDOUT_FILENO,str,strlen(str));
}

int main(int argc, char *argv[]){

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

  while (1){
    printf("I’m working!");
    fflush(stdout);
    sleep(1);
  }
}
