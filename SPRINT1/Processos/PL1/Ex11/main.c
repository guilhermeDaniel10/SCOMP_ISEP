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


int ARRAY_SIZE = 1000;
int main ()
{
  int numbers[ARRAY_SIZE];   /* array to lookup */
    time_t t;                /* needed to init. the random number generator (RNG) */
    int i, j, status;
    pid_t pid;

    /* intializes RNG (srand():stdlib.h; time(): time.h) */
    srand ((unsigned) time (&t));

   /* initialize array with random numbers (rand(): stdlib.h) */
    for (i = 0; i < ARRAY_SIZE; i++){
      numbers[i] = rand () % (255 + 1 - 0) + 0;

    }

    int range = ARRAY_SIZE / 5;


    /*alinea a
    for(i = 0; i < 5; i++){

      int ini = i * range;
      int fim = ini + range;


      pid = fork();
      if(pid == 0){
      int max = -1;
      for(j = ini; j < fim; j++){
        if(numbers[j] > max){
          max = numbers[j];
        }
      }
      printf("O valor máximo de porção do array é: %d\n", max);

      exit(0);
      }
    }

    while((pid = wait(NULL)) >= 0); */

    /*Alinea b,c e d */

    int maxArray[5];
    for(i = 0; i < 5; i++){

      int ini = i * range;
      int fim = ini + range;


      pid = fork();
      if(pid == 0){
      int max = -1;
      for(j = ini; j < fim; j++){
        if(numbers[j] > max){
          max = numbers[j];
        }
      }
      exit(max);

    } else {
      waitpid(pid, &status, 0);
      maxArray[i] = WEXITSTATUS(status);

      }
    }

    int max_value = -1;
    for(i = 0; i < 5; i++){
      if(max_value < maxArray[i]){
        max_value = maxArray[i];
      }
    }

    printf("Maior número é: %d\n\n", max_value);

    int half = ARRAY_SIZE / 2;
    int result[half];

    pid = fork();
    if(pid == 0){
      for(i = 0; i < half; i++){
        result[i]= ((float)numbers[i]/(float)max_value)*100;
        printf("RESULTADO: %d\n", result[i]);
      }
      exit(0);

    } else if(pid > 0){
      waitpid(pid, &status, 0);
      if(WIFEXITED(status)){

      for(i = half; i < ARRAY_SIZE; i++){
        result[i]=((float)numbers[i]/(float)max_value)*100;
        printf("RESULTADO: %d\n", result[i]);

      }
      exit(0);
      }
    }

    return 0;
  }
