#include <stdio.h>

void inverso(int *vec, int n, float *newVec){
  int i = 0;

  for(i = 0; i < n; i++){
    *(newVec + i) = (float) 1 / *(vec + i);
  }

}
int main(){

  puts("\nPrimeiro array:");
  int i = 0;
  int vec[] = {10, 11, 12, 13, 14, 15};
  int n = 6;
  float newVec[6];

  for(i = 0; i < n; i++){
    printf("%d - %d;",i, vec[i]);
  }
  inverso(vec, n, newVec);
  puts("\nSegundo array:");

  for(i = 0; i < n; i++){
    printf("%d - %f;",i, newVec[i]);
  }


  return 0;
}
