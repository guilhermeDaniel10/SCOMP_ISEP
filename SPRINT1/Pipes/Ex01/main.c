#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>

#define BUFFER_SIZE 80

int main(void){
  char read_msg[BUFFER_SIZE];
  int fd[2];
  pid_t pid;

  if(pipe(fd) == -1){
    perror("Pipe failed");
    return 1;
  }

  pid = fork();

  if(pid > 0){
    close(fd[0]);
    int p = getpid();
    printf("PID enviado pelo pai: %d\n", p);

    char s[120];

    sprintf(s,"%d", p);
    write(fd[1],s, strlen(s) + 1);

    close(fd[1]);
}else{ /* fecha a extremidade não usada */
   close(fd[1]);
   /* lê dados do pipe */
   read(fd[0], read_msg, BUFFER_SIZE);
   printf("PID lido pelo filho: %s\n", read_msg);
   /* fecha a extremidade de leitura */
   close(fd[0]); }


  return 0;
}
