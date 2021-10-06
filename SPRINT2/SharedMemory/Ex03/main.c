#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define TAMANHO 1000
//NOTA: Devido a um segmentation fault, foi usado apenas um array com TAMANHO elementos
typedef struct{
  int num;
  char scomp[255];
  int new_data;
} est;

void fillArray(est *bigArr, int size){
  int i = 0;
  for(i = 0; i < size; i++){
    bigArr[i].num = rand()%((100+1) - 1) + 1;
    strcpy(bigArr[i].scomp, "ISEP-SCOMP 2020");
  }
}

int main(){
  printf("A executar com recurso a memória partilhada (pode demorar alguns segundos).\n");
  shm_unlink("/ex03");
  time_t start, stop;
  long int totalMemP = 0;
  start = time(NULL);
  int fd, data_size = sizeof(est);
  est *shared;
  int pid;
  fd = shm_open("/ex03", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);

  if(fd == -1){
    printf("Erro a abrir");
    exit(EXIT_FAILURE);
  }

  ftruncate(fd, data_size);
  shared = (est *)mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

  shared->new_data = 0;
  pid = fork();

  if(pid > 0){
    int i = 0;
    for(i = 0; i < TAMANHO; i++){
      while(shared->new_data == 1){}
      shared->num = rand()%((100+1) - 1) + 1;
      strcpy(shared->scomp, "ISEP-SCOMP 2020");

      shared->new_data = 1;

    }
    wait(NULL);

  } else {
    est arra[TAMANHO];
    int j = 0;
    for(j = 0; j < TAMANHO; j++){
      while(shared->new_data == 0){}

      strcpy(arra[j].scomp, shared->scomp);

      shared->new_data = 0;

    }

    srand(time(NULL));
    stop = time(NULL);
    totalMemP = stop - start;

    printf("Com memoria partilhada demorou: %ld\n", totalMemP);

    if (munmap((void *)shared, data_size) < 0) {
        printf("Erro no munmap\n");
        exit(EXIT_FAILURE);
    }

    shm_unlink("/ex03");
    exit(EXIT_SUCCESS);
  }

  srand(time(NULL));
  stop = time(NULL);

  if (munmap((void *)shared, data_size) < 0) {
      printf("Erro no munmap\n");
      exit(EXIT_FAILURE);
  }

  if (close(fd) < 0) {
      printf("Error no close\n");
      exit(EXIT_FAILURE);
  }

  /* COM PIPES */
  puts("-----------------------------");


  int fd2[2];
  pid_t pid2;
  est read_msg;
  long int totalPipes = 0;
  time_t start2, stop2;

  if(pipe(fd2) == -1){
    perror("Pipe failed");
    return 1;
  }

  est t;
  pid2 = fork();

  start2 = time(NULL);

  if(pid2 > 0){
    int i = 0;
    close(fd2[0]);

    for(i = 0; i < TAMANHO; i++){

      int nn = rand()%((100+1) - 1) + 1;
      char *str = "ISEP-SCOMP 2020";

      t.num = nn;
      strcpy(t.scomp, str);

      write(fd2[1],&t, sizeof(t));


    }
    close(fd2[1]);
    wait(NULL);

  } else {
    close(fd2[1]);
    while(read(fd2[0], &read_msg, sizeof(read_msg)) > 0){
    }
    close(fd2[0]);

    exit(EXIT_SUCCESS);
  }

  stop2 = time(NULL);

  totalPipes = stop2 - start2;

  printf("Com pipes demorou: %ld", totalPipes);



  return 0;
}
