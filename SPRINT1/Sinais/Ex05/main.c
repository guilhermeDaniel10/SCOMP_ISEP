#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

void handler(int signo){
  if(signo == SIGQUIT){
    char *str = "I won’t let the process end with CTRL-\\";
    write(STDOUT_FILENO, str, strlen(str));

  }

 }

int main() {

  struct sigaction act;
  memset(&act, 0, sizeof(struct sigaction)); /*Preenche toda a struct a 0 */
  sigemptyset(&act.sa_mask);
  act.sa_handler = handler;
  sigaction(SIGQUIT, &act, NULL); /*Substituir aqui o sinal a ser lido */

  for(;;){
    printf("I Like Signal\n");
    sleep(1);
  }


}
