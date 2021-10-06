#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>
#include <ctype.h>

#define ARRAY_SIZE 1000
#define BUFFER_SIZE 120

int main(){

  int vec1[1000];
  int vec2[1000];
  int i = 0, status;

  pid_t pid;
  //time_t t;

  //srand ((unsigned) time (&t));

  for (i = 0; i < ARRAY_SIZE; i++){
    vec1[i] = i;
    vec2[i] = 1;
  }


  int range = ARRAY_SIZE / 5;
  printf("Range: %d\n", range);


  int fd[5][2];
  int j;
  int vec3[1000];

  for(j = 0; j < 5; j++){
    int start = j * 200;
    int final = start + range;
    int k = 0;

    int soma;

    if(pipe(fd[j]) == -1){
      perror("Erro");
    }

    pid = fork();

    if(pid == 0){
      close(fd[j][0]);

      puts("----------------------------------");

      for(k = start; k < final; k++){
        soma =+ vec1[k] + vec2[k];
        printf("MENSAGEM A ENVIAR: %d\n", soma);
        write(fd[j][1], &soma, sizeof(soma));
      }
      //sprintf(msg, "%d", soma);

      close(fd[j][1]);
      exit(20);
    } else if(pid > 0){
      wait(&status);
      int read_msg;

      close(fd[j][1]);
      int index;

      while(read(fd[j][0], &read_msg, sizeof(read_msg)) > 0){
        vec3[index] = read_msg;
        index++;
      }
    }
    close(fd[j][0]);
  }

  int h2;

  for(h2 = 0; h2 < 1000; h2++){
    printf("Numeros: %d\n", vec3[h2]);
  }

  return 0;
}
