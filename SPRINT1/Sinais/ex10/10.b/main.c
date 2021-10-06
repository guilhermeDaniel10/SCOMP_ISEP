#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

static int flag;

int countCharacters(char *stringKeyboard);

int sigaction(int sig,
const struct sigaction * act,
struct sigaction * oact);

void handle_CHILD(int signo, siginfo_t *sinfo, void *context) {
  flag = 0;
}

int main(void){
      /* sigaction struct. */
      struct sigaction act;

      /* Clear the act variable. */
      memset(&act, 0, sizeof(struct sigaction));

      /* All signals blocked */
      sigfillset(&act.sa_mask);

      /* Receive and handle SIGUSR1 */
      act.sa_sigaction = handle_CHILD;
      act.sa_flags = SA_SIGINFO;
      sigaction(SIGCHLD, &act, NULL);

      int qtdCharacters = 0;
      char stringKeyboard[100];
      memset(stringKeyboard,0,sizeof(stringKeyboard));

      int p = fork();
      flag = -1;

      if (p !=  0) {
        sleep(10);
        if (flag == 0) {
          sleep(20);
          printf("Successful execution!\n");
        } else {
          /*son never exits so we must kill him*/
          kill(p,SIGINT);
          printf("To slow, that is why the program will end!\n");
        }

      } else {
        /* You have 10 seconds before interrupting the input! */
        printf("Input the desirable string:\n");
        fgets(stringKeyboard,100,stdin);
        qtdCharacters = countCharacters(stringKeyboard);
        printf("You successfully inputed the message with %d characters!!!\n", qtdCharacters);
        exit(qtdCharacters);
      }

}

int countCharacters(char *stringKeyboard){
  int qtdCharacters = 0;
  for (int i = 0; i < strlen(stringKeyboard); i++) {
    if (stringKeyboard[i] != 32 && stringKeyboard[i] != 10) {
      qtdCharacters++;
    }
  }
  return qtdCharacters;
}
