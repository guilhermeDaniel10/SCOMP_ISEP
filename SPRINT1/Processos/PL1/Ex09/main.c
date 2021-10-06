/* libraria do printf */
#include <stdio.h>
/* librarias para os processos */
#include <sys/types.h> /*pid_t */
#include <unistd.h> /*fork */
/*librarias do wait */
#include <sys/types.h>
#include <sys/wait.h>
/*libraria do exit */
#include <stdlib.h>
#include <time.h>


int main(void){

  pid_t pid;
  int i, j, status;

  for(i = 0; i < 10; i++){
    pid = fork();
    if(pid == 0){

      int numIn = (i*100) + 1;
      int numFi = (i * 100) + 100;

      for(j = numIn; j <= numFi; j++){
        printf("%d\n",j);
      }
      exit(0);
    }
  }
  while ((pid = wait(NULL)) >= 0); /*O wait espera por um erro para poder parar a execução dos processos*/

  puts("Terminaram os filhos");

  return 0;
}

/* a) O output não está ordenado, sendo que não se sabe qual processo é executado
primeiro e que múltiplos processos filhos podem ser executados ao mesmo tempo */
