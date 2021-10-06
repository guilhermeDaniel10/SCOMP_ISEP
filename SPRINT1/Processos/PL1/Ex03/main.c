/* libraria do printf */
#include <stdio.h>
/* librarias para os processos */
#include <sys/types.h> /*pid_t */
#include <unistd.h> /*fork */

int main(void){

    fork(); fork(); fork(); /* Por cada chamada do fork são criados 2^n processos, neste caso, serão criados 2^3 processos,
    o que dá um total de 8 processos */
    printf("SCOMP!\n");



  return 0;
}

/*a) Por cada chamada do fork são criados 2^n processos, neste caso, serão criados 2^3 processos,
o que dá um total de 8 processos */

/*c) "SCOMP" é impresso a mesma quantidade de processos existentes no programa, sendo que este está fora de qualquer
condição de verificação do pid do processo. Concluindo, SCOMP será impresso 8 vezes. */
