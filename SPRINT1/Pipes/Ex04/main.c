/* libraria do printf */
#include <stdio.h>
/* librarias para os processos */
#include <sys/types.h> /*pid_t */
#include <unistd.h> /*fork and pipe*/
/*librarias do wait */
#include <sys/types.h>
#include <sys/wait.h>
/*libraria strlen*/
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 800
#define LINE_SIZE 100
char fileName[10] = "output.txt";

int main(void){

  /*string para a leitura*/
  char read_msg[BUFFER_SIZE];

  /*string para Escrita*/
  //char write_msg[BUFFER_SIZE];
  /*multiple lines*/
  //char lines[LINE_SIZE];
  char *lines = NULL;

  /*Tamanho da linha*/
  ssize_t read_s;
  size_t tam = 0;

  /**/
  FILE *fp;

  /*Pipe*/
  int fd[2];

  /*Process Identifier*/
  pid_t pid;
  int status;

  /* cria o pipe */
  if(pipe(fd) == -1){
    perror("Pipe failed");
    return 1;
  }

  pid = fork();

  /*no caso de não conseguir fazer mais forks*/
  if(pid == -1){
    perror("Fork falhou!");
    exit(1);
  }

  if(pid > 0){
    /* fecha a extremidade não usada pai só escreve */
    close(fd[0]);

    /*read mode*/
    fp = fopen(fileName, "r");
    if (!fp) {
      printf("Could not open file. Exiting application!");
      return 1;
    }

    /*reads from a txt file to a String line at a time
    and adds all in the main string write_msg*/
    /*OU*/
    /**/
    //while(fgets(lines, sizeof(lines), fp) != NULL) {
    /*The getline function reads an entire line from a stream*/
    while((read_s = getline(&lines, &tam, fp)) != -1) {
      //strcat(write_msg, lines);
      //strcpy(lines, "");
      //write(fd[1], lines, sizeof(lines));
      write(fd[1], lines, read_s);
    }
    /*closes connection to the txt file*/
    fclose(fp);

    /*message read*/
    //printf("O pai recebeu o seguinte texto:\n%s\n", write_msg);

    /* escreve no pipe */
    //write(fd[1], write_msg, sizeof(write_msg));

    /* fecha a extremidade de escrita */
    close(fd[1]);

    /*parent must wait for child to end*/
    waitpid(pid,&status,0);
    printf("Filho terminou!\n");

  } else {

    /* fecha a extremidade não usada, filho só lê */
    close(fd[1]);

    /* lê string do pipe */
    /**/
    while(read(fd[0], read_msg, sizeof(read_msg))){
      printf("\nFilho leu a seguinte linha:\n%s\n", read_msg);
    }

    /* fecha a extremidade de leitura */
    close(fd[0]);
    exit(1);
  }

  return 0;
}
