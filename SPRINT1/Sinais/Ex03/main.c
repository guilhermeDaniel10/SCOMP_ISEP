#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>


void mensagem(int signo){
  if(signo == SIGUSR1){
    char str[120];
    sprintf(str, "SIGUSR1 has been captured and was sent by the process with PID %d\n", (int)getpid());
    write(STDOUT_FILENO, str, strlen(str));

  }

 }

int main(int argc, char *argv[]){

  struct sigaction act;
  memset(&act, 0, sizeof(struct sigaction)); /*Preenche toda a struct a 0 */
  sigemptyset(&act.sa_mask);
  act.sa_handler = mensagem;
  sigaction(SIGUSR1, &act, NULL);

  int i;
  printf("PROCESSO:%d\n", getpid());
  for(i = 1;; i++){

    if(i % 2== 0){
      raise(SIGUSR1);
    }
    printf("A correr...\n");
    sleep(3);
    fflush(stdout); //Força o output a correr

  }





  return 0;

}
