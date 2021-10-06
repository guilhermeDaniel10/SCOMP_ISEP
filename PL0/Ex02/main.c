#include <stdio.h>

void subtracao(int *vec, int n, int sub){
  int res = 0;
  int i = 0;

  for (i = 0; i < n; i++) {
    res = *(vec + i) - sub;
    *(vec + i) = res;
  }

}

int main(){
  int vec[] = {10,11,12,13,14,15};
  int n = 6;
  int sub = 2;

  int i = 0;
  puts("Pre sub\n");

  for(i = 0; i < n; i++){
    printf("%d - %d;", i, vec[i]);
  }
  subtracao(vec, n, sub);
  puts("\nPos sub\n");

  for(i = 0; i < n; i++){
    printf("%d - %d;", i, vec[i]);
  }



  return 0;
}
