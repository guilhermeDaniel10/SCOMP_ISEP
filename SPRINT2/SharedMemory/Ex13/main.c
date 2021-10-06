#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define MAX 1000

typedef struct{
    char path[65];
    char word[30];
    int oco;
} fic;

/*Determina o numero de ocorrencias de uma palavra um ficheiro*/
int find_word(char *path, char *word){
  FILE *fic = fopen(path, "r");

  if(fic == NULL){
    printf("Erro a abrir o ficheiro %s\n", path);
    exit(EXIT_FAILURE);
  }

  char str[MAX];
  char *pos;
  int index, count;
  count = 0;
  while((fgets(str, MAX, fic)) != NULL){
    index = 0;
    while((pos = strstr(str + index, word)) != NULL){
      index = (pos - str) + 1;
      count++;
    }
  }
  return count;
}

/*Cada ficheiro tem um numero de palavras especificas a procurar dependendo do seu
numero */
char *word_to_search(int index){
  char s[50];
  char *str = s;
  if(index == 1 || index == 4 || index == 6 || index == 8 || index == 10){
    strcpy(str, "ficheiro");
  } else if(index == 3 || index == 5 || index == 7 || index == 9){
    strcpy(str, "palavra");
  } else {
    strcpy(str, "frase");
  }
  return str;
}

int main(){

  int fd, data_size = sizeof(fic);
  fic *shared;

  if(shm_unlink("/ex13") < 0){
    printf("Sem necessidade de fechar.\n");
    exit(EXIT_FAILURE);
  }

  fd = shm_open("/ex13", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);

  if(fd == -1){
    printf("Erro a abrir");
    exit(EXIT_FAILURE);
  }
  ftruncate(fd, data_size);
  shared = (fic *)mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

  int i = 0;
  pid_t pid;


  for(i = 1; i <= 10; i++){
    char str[65];
    sprintf(str, "file%d.txt", i);
    /*Faz a atribuicao do ficheiro a estrutura da memoria partilhada */
    strcpy(shared->path, str);
    char *w = word_to_search(i);
    strcpy(shared->word, w);
    pid = fork();

    if(pid == -1){
      perror("Erro no fork.\n");
      exit(-1);
    }

    if(pid == 0){
      int nmrOco = find_word(shared->path, shared->word);
      shared->oco = nmrOco;

      exit(1);
    } else {
      wait(NULL);
      printf("Ficheiro: %s\n", shared->path);
      printf("Palavra: %s\n", shared->word);
      printf("Numero de ocorrencias: %d\n", shared->oco);
      puts("-------------------");
    }
  }



  if (munmap(shared, data_size) < 0) {
      printf("Erro no munmap\n");
      exit(EXIT_FAILURE);
  }

  if (close(fd) < 0) {
      printf("Erro no close\n");
      exit(EXIT_FAILURE);
  }


  return 0;
}
