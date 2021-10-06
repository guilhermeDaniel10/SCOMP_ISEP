#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int n = 6;

int spawn_childs(int n);

int main(void){

  int id =  spawn_childs(n);

  if (id > 0) {
      printf("I'm the son s%d and my ID is = %d\n",id/2,id);
      exit(id);
  } else {
      printf("I'm the father and my ID is = %d\n",id);
  }
}

int spawn_childs(int n){
  pid_t p;
  for (int i = 0; i < n; i++) {
    p = fork();
    if (p == 0) {
      return (i+1)*2;
    }
  }

  while ((p = wait(NULL)) >= 0); /*Espera que os filhos sejam todos executados */
  return 0;
}
