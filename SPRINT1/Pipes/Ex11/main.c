#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>
#include <ctype.h>

#define NUMBER_OF_CHILDREN 5

int spawn_childs(int nbrChilds){
    pid_t pid;
    int i;
    for (i = 0; i < nbrChilds; i++){
        pid = fork();
        if (pid < 0) {
          return -1;
        } else if (pid == 0) {
          return i + 1;
        }
    }
    return 0;
}


int generateNumber(int pid){

  srand (pid);
  int num = (int)(rand() % (500 - 1 + 1)) + 1;
  return num;
}

int main(){

  int i = 0;
  int fd[6][2];

  for(i = 0; i < 6; i++){
    if(pipe(fd[i]) == -1){
      perror("Erro no pipe");
      return 1;
    }
  }
  int id = spawn_childs(NUMBER_OF_CHILDREN);

  //Com o uso da funcao spawn_childs, o ip do pai vai ser 0

  switch(id)
  {
    case 0:
      close(fd[id][0]);
      close(fd[5][1]);
      int numP = generateNumber(getpid());
      int ultimo;
      printf("O pai gerou e enviou: %d e tem PID: %d\n", numP, getpid());
      puts("----------------------");
      write(fd[id][1], &numP, sizeof(numP));
      read(fd[5][0], &ultimo, sizeof(ultimo));

      printf("O pai recebe o maior numero gerado: %d\n", ultimo);
      close(fd[id][1]);
      close(fd[5][0]);

    break;

    default:

    close(fd[id-1][1]);
    close(fd[id][0]);

    int num1 = generateNumber(getpid());
    int readP1;

    read(fd[id-1][0], &readP1, sizeof(readP1));
      puts("----------------------");
    printf("Filho %d com PID: %d \n", id, getpid());
    printf("Filho %d recebeu: %d \n", id, readP1);
    printf("Filho %d gerou: %d \n", id, num1);

    if(num1 > readP1){
      printf("Filho %d enviou: %d \n", id, num1);
      write(fd[id][1], &num1, sizeof(num1));
    } else {
      printf("Filho %d enviou: %d \n", id, readP1);
      write(fd[id][1], &readP1, sizeof(readP1));
    }


    close(fd[id-1][0]);
    close(fd[id][1]);
    exit(1);

  }






  return 0;
}
