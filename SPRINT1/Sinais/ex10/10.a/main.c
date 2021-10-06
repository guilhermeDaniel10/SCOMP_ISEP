#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

int countCharacters(char *stringKeyboard);

int sigaction(int sig,
const struct sigaction * act,
struct sigaction * oact);

void handle_ALARM(int signo) {
  int pid = getpid();
  char *str = "To slow, that is why the program will end!\n";
  write(STDOUT_FILENO, str, strlen(str));
  kill(pid,SIGINT);
}

int main(void){

      struct sigaction act;
      memset(&act, 0, sizeof(struct sigaction));
      /* No signals blocked */
      sigemptyset(&act.sa_mask);
      act.sa_handler = handle_ALARM;
      sigaction(SIGALRM, &act, NULL);
      /* SIGALRM is now set to be catched and handled. */

      int qtdCharacters = 0;
      char stringKeyboard[100];
      memset(stringKeyboard,0,sizeof(stringKeyboard));

      alarm(10);
      /* You have 10 seconds before interrupting the input! */
      printf("Input the desirable string:\n");
      fgets(stringKeyboard,100,stdin);

      /* will cancel any currently active alarm */
      alarm(0);

      qtdCharacters = countCharacters(stringKeyboard);
      printf("You successfully inputed the message with %d characters!!!\n", qtdCharacters);
      sleep(20);
      printf("Successful execution!\n");
}


int countCharacters(char *stringKeyboard){

  int qtdCharacters = 0;

  for (int i = 0; i < strlen(stringKeyboard); i++) {

    if (stringKeyboard[i] != 32 && stringKeyboard[i] != 10) {
      /*printf("%c\n",stringKeyboard[i]);*/
      qtdCharacters++;
    }
  }

  return qtdCharacters;
}
