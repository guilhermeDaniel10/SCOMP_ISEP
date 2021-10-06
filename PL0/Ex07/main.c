#include <stdio.h>
#include <string.h>

char equal(char *s){
  int sS = strlen(s);
  char cp[sS];
  char vec[sS];
  char *newVec = vec;
  char *copy = cp;
  int i = sS;

  for(i = 0; i <= sS; i++){
    if(*(s+i) != ' '){
      printf("%d-%c\n", i,*(s+i));
      *(copy + i) = *(s + i);
    }

  }

  for(i = sS; i >= 0; i--){
    if(*(s+i) != ' '){
      printf("%c\n", *(s+i));
      *(newVec + i) = *(s + i);

    }

  }

  for(i = 0; i < strlen(copy); i++){
    printf("%c",*(copy+i));
  }


  if(strcmp(s, copy) == 0){
    return '1';
  }
  return '0';

}

char canBeRead(char *a, char *b){
  if(equal(a) == '1' && equal(b) == '1'){
    return '1';
  }
  return '0';
}

int main(){

  char *a = "anotaram a data da maratona";
  char *b = "a torre da derrota";

  printf("%c\n", canBeRead(a,b));

  return 0;
}
