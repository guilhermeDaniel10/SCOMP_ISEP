#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <sys/wait.h>

int counter = 0;

int sigaction(int sig,
const struct sigaction * act,
struct sigaction * oact);

void handle_USR1(int signo, siginfo_t *sinfo, void *context) {
  /**handler to take more that one sec*/
  sleep(2);
  counter++;
  /*Alternativa mais segura ao printf*/
  char str[80];
  sprintf(str, "SIGUSR1 signal captured, USR1_COUNTER = %d\n", counter);
  write(STDOUT_FILENO,str,strlen(str));
  sleep(2);
}

int main(int argc, char *argv[]){

  /* sigaction struct. */
  struct sigaction act;

  /* Clear the act variable. */
  memset(&act, 0, sizeof(struct sigaction));

   /* All signals blocked line d) */
   /*sigfillset(&act.sa_mask);*/

   /* No signals blocked - line e)*/
   sigemptyset(&act.sa_mask);

  /* Receive and handle SIGUSR1 */
  act.sa_sigaction = handle_USR1;
  act.sa_flags = SA_SIGINFO;
  sigaction(SIGUSR1, &act, NULL);

 /*Waits 10ms*/
  struct timespec tim;
  tim.tv_sec = 0;
  tim.tv_nsec = 10000000;

  pid_t pid;
  pid = fork();

  if (pid == 0) {
    int parent = (int) getppid();
    for (int signal = 1; signal < 13; signal++) {
      printf("\nsignals!");
      fflush(stdout);
      if (signal==1) {
          kill(parent,SIGUSR1);
      }else {
          kill(parent,SIGUSR2);
      }
        nanosleep(&tim,NULL);
    }
  } else {
    printf("Reached Father!");
    fflush(stdout);
  }

  while (1){
    printf("I’m working!");
    fflush(stdout);
    sleep(1);
  }
}
