#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <semaphore.h>

#define MAX_TIME 30

int main(){
	sem_t *sem_west, *sem_ponte;
	if ((sem_west = sem_open("sem_west", O_EXCL, 0644, 0)) == SEM_FAILED || (sem_ponte = sem_open("sem_ponte", O_EXCL, 0644, 1)) == SEM_FAILED) {
		perror("Erro a criar o semaforo.\n");
		exit(0);
	}

	sem_post(sem_west);

	int value;
	sem_getvalue(sem_west, &value);
	printf("West -> carro %d chegou ao semáforo\n", value);
	if(value == 1){
    sem_wait(sem_ponte);
  }



  int incr = 0;
  do {
    incr++;
    sleep(1);
    printf("West -> carro numero %d está a passar a ponte no km %d\n", value, incr);
  } while(incr != MAX_TIME);


	printf("West: -> carro numero %d está a sair da ponte\n", value);
	sem_wait(sem_west);

	sem_getvalue(sem_west, &value);
	if(value == 0)
		sem_post(sem_ponte);

	return 0;
}
