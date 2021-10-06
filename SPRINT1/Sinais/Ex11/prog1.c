#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

int pid = 0;

void handle_alarm(int signo){
  char *str = "\nThe command prog1.c didn't end in its allowed time!\n";
  write(STDOUT_FILENO,str,strlen(str));
  if(pid != 0){
    kill(pid, SIGKILL);
  }
}

int main(){

  struct sigaction act;

  memset(&act, 0, sizeof(struct sigaction));
  sigemptyset(&act.sa_mask);
  act.sa_handler = handle_alarm;
  sigaction(SIGALRM, &act, NULL);

  pid = getpid();


  printf("Programa 1\n");
  sleep(8);
  printf("\nExecucao do prog1 terminou corretamente!\n");

}
