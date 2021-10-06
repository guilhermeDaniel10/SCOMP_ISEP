#include <stdio.h>
#include <stdlib.h>


int nVizinhos(int *vec, int n){
  int i = 0;
  int count = 0;

  for(i = 1; i < n - 1; i++){
    int j = i - 1;
    int k = i + 1;
    if(*(vec + i) > *(vec + j) && *(vec + i) > *(vec + k)){
      count++;
    }
  }
  return count;
}

void copyVec(int *vec, int *cp, int n){
  int i = 0;
  int l = 0;

  for(i = 1; i < n - 1; i++){
    int j = i - 1;
    int k = i + 1;
    if(*(vec + i) > *(vec + j) && *(vec + i) > *(vec + k)){
      *(cp + l) = *(vec + i);
      l++;
    }
  }
}

int main(){

  int n;
  int *vec;
  printf("%s\n", "Quantos inteiros?");
  scanf("%d", &n);

  vec = calloc(n, sizeof(int));
  int i = 0;

  for(i = 0; i < n; i++){
    puts("Numero:");
    scanf("%d", &(*(vec + i)));
  }

  int num = nVizinhos(vec, n);
  int cp[num];
  int *copy = cp;

  copyVec(vec, copy, n);

  puts("---------");
  for(i = 0;i < num; i++){
    printf("%d - %d;" ,i, *(copy + i));
  }


  free (vec);

  return 0;
}
