#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>
#include <ctype.h>
#define BUFFER_SIZE 256

void func(char * temp) {
  char * name;
  name = strtok(temp,":");

  char *s = name;
  while (*s) {
    *s = toupper((unsigned char) *s);
    s++;
  }
}


int main()
{

  char msg[BUFFER_SIZE];

  int up[2], down[2];

  pipe(up);pipe(down);


  pid_t pid = fork();

   if(pid > 0) {

      close(down[0]);
      close(up[1]);

      read(up[0], msg, BUFFER_SIZE);

      printf("Mensagem recebida do filho para o pai: %s\n", msg);

      close(up[0]);
      func(msg);

      write(down[1], msg, strlen(msg) + 1);
      close(down[1]);

  }
  else if( pid == 0 ) {


      close(up[0]);
      close(down[1]);

      printf("Por favor insira uma String: ");
      fgets(msg, BUFFER_SIZE, stdin);


      write(up[1], msg, strlen(msg) + 1);

      close(up[1]);

      read(down[0], msg, strlen(msg) + 1);
      printf("Mensagem recebida do pai para o filho: %s\n", msg);
      close(down[0]);


  }

  else {
      perror("Erro!\n"); //fork()
  }

  return 0;
}
