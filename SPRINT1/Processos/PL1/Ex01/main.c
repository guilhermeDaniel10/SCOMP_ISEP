/* libraria do printf */
#include <stdio.h>
/* librarias para os processos */
#include <sys/types.h> /*pid_t */
#include <unistd.h> /*fork */

int main(void){
  int x = 1;
  pid_t p = fork();
  /*Fork:
  -cria uma cópia exata do processo original */

  if(p == 0){ /*Código do filho */
    x = x+1;
    printf("\n1. x = %d\n", x);
    puts("Processo filho");
  } else { /*Código para o pai e para um possível erro pois erro é pid == -1 */
    x = x-1;
    printf("\n2. x = %d\n", x);
    puts("Processo pai");
  }
  printf("3. %d; x = %d\n", p, x);  /*Ambos os processos pai/filho chegarao a esta linha */
  return 0;
}

/*a) De acordo com a teoria do fork, "Geralmente o pai e o filho precisam de excutar
código diferente, logo o fork retorna 0 para o filho e retorn o pid do filho para o pai",
logo, imaginando que o primeiro processo a ser executado era o do filho, o output esperado
seria: 1. x = 2 (...) 3. 0; x = 2   .Depois de ser executado o filho, preve-se então que
o pai tenha o pid do filho, sendo o output esperado: 2. x = 0; (...) 3. 1234; x = 0
Nota: Não se sabe qual o processo que será executado pelo CPU primeiro, pelo que poderia
aparecer primeiro o output do pai seguido do filho, ou o inverso.*/

/*b) Sim, será sempre garantido que será visto primeiro o output da linha 11,
sendo que primeiro é executada a condição do valor do Process Id, quer para o pai
ou para o filho, e só depois é executada a linha 11. O output do printf da linha 11
irá aparecer duas vezes, tanto depois da execução do processo pai como após a e do
processo filho */
