#include <stdio.h>
#include <stdlib.h>


int main(){

  char **nomes;
  nomes = calloc(5, 1+sizeof(char*));

  int i = 0;

  for(i = 0; i < 5; i++){
    puts("Nome");
    scanf("%s", nomes[i]);
  }

  for(i = 0; i < 5; i++){

    printf("-----%s\n", (nomes + i));


  }

  free(nomes);

  return 0;
}
