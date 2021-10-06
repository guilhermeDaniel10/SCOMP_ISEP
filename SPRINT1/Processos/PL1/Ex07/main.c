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



#define ARRAY_SIZE 1000
int main ()
{
  int numbers[ARRAY_SIZE];   /* array to lookup */
    int n;                   /* the number to find */
    time_t t;                /* needed to init. the random number generator (RNG) */
    int i;

    /* intializes RNG (srand():stdlib.h; time(): time.h) */
    srand ((unsigned) time (&t));

   /* initialize array with random numbers (rand(): stdlib.h) */
    for (i = 0; i < ARRAY_SIZE; i++)
      numbers[i] = rand () % 10000;

    /* initialize n */
    n = rand () % 10000;

    pid_t pid = fork();
    int half = ARRAY_SIZE / 2;
    int found = 0;
    int j = 0;
    int total = 0;
    int status;

    /* Processo filho */
    if(pid == 0){
      for(j = 0; j < half; j++){
        printf("%d Nmr: %d\n" , j, numbers[j]);
        if(n == numbers[j]){
          /*Incremento das ocorrências do número n */
          found+=1;
        }
      }
      /*É retornado o contador com o número de ocorrências do número n na primeira metade do array */
      exit(found);
    } else {
      /*Espera que o processo filho termine */
      waitpid(pid, &status, 0);
      for(j = half; j < ARRAY_SIZE; j++){
        printf("%d Nmr: %d\n" , j, numbers[j]);
        if(n == numbers[j]){
          /*Incremento das ocorrências do número n */
          found+=1;
        }
    }
  }
    /*WEXITSTATUS é o valor retornado pelo exit do processo filho */
    total = WEXITSTATUS(status) + found;
    printf("\nTotal: %d", total);

    return 0;
  }
