#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>



void handler(int signo, siginfo_t *sinfo, void *context){
  sigset_t pending;
  int i;
  for(i = 1; i <= 64; i++ ){
    if(i != 9 && i != 19 && i != 32 && i != 33){
    raise(i);
    sigpending(&pending);
    if(sigismember(&pending,  i)){
      char str[120];
      sprintf(str, "SIGNAL: %d\n", i);
      write(STDOUT_FILENO, str, strlen(str));
    }

  }
}
  sigprocmask(SIG_UNBLOCK, NULL, 0);
}


int main(){
  struct sigaction act;

  memset(&act, 0, sizeof(struct sigaction));
  sigfillset(&act.sa_mask);

  act.sa_sigaction = handler;
  act.sa_flags = SA_SIGINFO;
  sigaction(SIGUSR1, &act, NULL);

  for(;;){
    raise(SIGUSR1);
    printf("RUNNING\n");
    sleep(1);

  }


}
