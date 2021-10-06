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
  int status;
  int n;

  if(pid > 0){
    close(fd[0]);
    char *msg1 = "Hello World!\n";
    char *msg2 = "Goodbye!\n";

    write(fd[1],msg1, strlen(msg1) + 1);
    write(fd[1],msg2, strlen(msg2) + 1);

    close(fd[1]);

  } else if(pid == 0){
    close(fd[1]);

    while((n = read(fd[0], read_msg, BUFFER_SIZE)) > 0){
      write(STDOUT_FILENO, read_msg, n);
    }

    close(fd[0]);
    exit(25);

  } else {
    perror("Erro!");
  }

  int pidFilho = waitpid(pid, &status, 0);
  printf("PID %d com valor de saída %d\n", pidFilho, WEXITSTATUS(status));


  return 0;
}
