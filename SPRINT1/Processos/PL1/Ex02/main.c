/* libraria do printf */
#include <stdio.h>
/* librarias para os processos */
#include <sys/types.h> /*pid_t */
#include <unistd.h> /*fork */
/*librarias do wait */
#include <sys/types.h>
#include <sys/wait.h>


int main(void){
  /*Este printf é executado antes de qualquer
  processo por isso está antes do fork()*/
  printf("I'm..\n");
  /*Com a chamada do fork, neste momento
  apenas estão criados dois
  processos- processo pai e processo filho */
  int pid = fork();

  /* Quem executa o seguinte printf
  é o processo pai fazendo um novo fork
  para o I'll never join you! ser impresso
  mais uma vez*/
  if(pid > 0){
      printf("the..\n");
      pid = fork();
  }

  /* Quem executa o seguinte printf
  é o processo pai fazendo um novo fork
  para o I'll never join you! ser impresso
  mais uma vez*/
  if(pid > 0){
      printf("father!\n");
      pid = fork();
  }

  /* tendo em conta que são criados
  três processos filhos este printf
  I'll never join you! será impresso
  3 vezes*/
  if(pid == 0){
      printf("I'll never join you!\n");
  }

  return 0;
}
