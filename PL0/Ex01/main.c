#include <stdio.h>

int somaPares(int *vec, int n){
  int i = 0;
  int soma = 0;
  for (i = 0; i < n; i++) {
    if((*(vec + i) & 1) == 0){
      soma += *(vec + i);
    }
  }
  return soma;
}

int main(){

  int vec[] = {2,3,7,8};
  int sum = somaPares(vec, 4);

  printf("\nA soma é:%d\n", sum);


  return 0;
}
