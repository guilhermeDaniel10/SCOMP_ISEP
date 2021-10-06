#include <stdio.h>
#include <string.h>

void copiar(char *vec1, char *vec2){
  int a = strlen(vec1);
  int i = 0;

  for(i = 0; i < a; i++){
    *(vec2 + i) = *(vec1 + i);
  }

}

int main(){

  char *str = "Bom dia";
  char c[30];
  char *copia = c;
  puts("Primeira frase:");
  printf("%s\n", str);
  copiar(str, copia);
  puts("Copia:");
  printf("%s\n", copia);


  return 0;
}
