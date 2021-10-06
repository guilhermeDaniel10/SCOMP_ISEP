#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

volatile sig_atomic_t childSucceded;
volatile sig_atomic_t childFinish;
volatile sig_atomic_t qtdProcessos;
volatile sig_atomic_t esperar = 0;
volatile sig_atomic_t success = -1;
volatile sig_atomic_t ordemDeProsseguir = -1;
int allSons[50];

int sigaction(int sig, const struct sigaction * act, struct sigaction * oact);

/*Handles SIGUSR2 so that the parent knows that the child didnt found data*/
void handle_USR2(int signo, siginfo_t *sinfo, void *context) {
  qtdProcessos++;
  /*Alternativa mais segura ao printf*/
  char *str = "\n------------------Processo com falha!-----------------\n";
  write(STDOUT_FILENO,str,strlen(str));
}

/*Handles SIGUSR1 so that the parent knows that one child found data*/
void handle_USR1(int signo, siginfo_t *sinfo, void *context) {
  success = 0;
  childSucceded++;
  qtdProcessos++;
  /*Alternativa mais segura ao printf*/
  char *str = "\n------------------Processo retornou com Sucesso!-----------------\n";
  write(STDOUT_FILENO,str,strlen(str));
  esperar = 1;
}

/*Handles SIGCONT for the child processes to continue*/
void handle_CONT(int signo, siginfo_t *sinfo, void *context) {
  /*Alternativa mais segura ao printf*/
  char *str = "\n------------------Processo filho continua!-----------------\n";
  write(STDOUT_FILENO,str,strlen(str));
  ordemDeProsseguir = 1;
}

/*Handles SIGCHILD so that the father can know how many have returned*/
void handle_CHLD(int signo, siginfo_t *sinfo, void *context) {
  childFinish++;
  /*Alternativa mais segura ao printf*/
  char *str = "\n------------------Filho Terminou!-----------------\n";
  write(STDOUT_FILENO,str,strlen(str));
}

/*Randomly generated values to implement exercise*/
int simula1(int nbrChld);

/*Randomly generated values to implement exercise*/
int simula2(int nbrChld);

/*i. In that case, that 25 processes have finished and
 none of the searches have been done successfully.*/
void stopEveryThing();

/*ii. When one child process has done a successful search, the parent process
sends a signal to all the remaining running children processes to start the execution
of the simula2()*/
void newTask();

int main(void) {
  /* sigaction struct. */
  struct sigaction act;

  /* Clear the act variable. */
  memset(&act, 0, sizeof(struct sigaction));

  /* All signals blocked */
  sigfillset(&act.sa_mask);

  /* Receive and handle SIGUSR1 */
  act.sa_sigaction = handle_USR1;
  act.sa_flags = SA_SIGINFO;
  sigaction(SIGUSR1, &act, NULL);

  /* Receive and handle SIGUSR2 */
  act.sa_sigaction = handle_USR2;
  act.sa_flags = SA_SIGINFO;
  sigaction(SIGUSR2, &act, NULL);

  /* Receive and handle SIGCONT */
  act.sa_sigaction = handle_CONT;
  act.sa_flags = SA_SIGINFO;
  sigaction(SIGCONT, &act, NULL);

  /* Receive and handle SIGCHLD */
  act.sa_sigaction = handle_CHLD;
  act.sa_flags = SA_SIGINFO;
  sigaction(SIGCHLD, &act, NULL);

  /*Process ID variable*/
  int p;

  qtdProcessos = 0;
  childFinish = 0;
  childSucceded = 0;

  for (int i = 0; i < 50; i++) {
    p = fork();

    /*in case we cannot do more forks*/
    if(p == -1){
      perror("Fork falhou!");
      exit(1);
    }

    if (p > 0) {
      /* saves the pid of all the the children */
      allSons[i] = p;
    } else {
      printf("Filho %d parte 1\n",i);
      fflush(stdout);
      int parentPID = getppid();
      int valido1 = simula1(i+1);
      sleep(2);
      if (valido1 == 0) {
        kill(parentPID,SIGUSR1);
      } else if(valido1 == -1 ) {
        kill(parentPID,SIGUSR2);
      } else if(valido1 == 1){
        printf("Father said to move on!");
        fflush(stdout);
      }
      /*Waits untill the father sends an "ok" signal to proceed*/
      esperar = 0;
      while(!esperar);
      printf("Filho %d parte 2\n",i);
      int valido2 = simula2(i+1);
      if (valido2 == 0) {
        kill(parentPID,SIGUSR1);
      } else {
        kill(parentPID,SIGUSR2);
      }
      exit(1);
    }
  }

  while(childFinish != 50){
    printf("qtdProcessos = %d\n",qtdProcessos);
    fflush(stdout);
    if (qtdProcessos == 25 && success == -1) {
      printf("Entrou 1\n");
      fflush(stdout);
      stopEveryThing();
    } else if (qtdProcessos < 25 && success == 0) {
      printf("Entrou 2\n");
      fflush(stdout);
      newTask();
    }
    /* Wait for some signal */
    pause();
  }

  /*Waits for all processes to finish certifies that everything has
  stopped*/
  while ((p = wait(NULL)) >= 0);

}

/*Função pedida no enunciado*/
int simula1(int nbrChld){
  int p = getpid();
  int upper = 10000;
  int lower = 0;

  printf("Child%d doing simula1\n",nbrChld);
  fflush(stdout);

  for(int i = 0; i < 100; i++){
    if (ordemDeProsseguir != 1) {
      int rnd = (int) (rand() % (upper - lower + 1)) + lower;
      if (rnd == p) {
        return 0;
      }
    } else {
      return 1;
    }

  }
  return -1;
}

/*Função pedida no enunciado*/
int simula2(int nbrChld){
  int p = getpid();
  int upper = 10000;
  int lower = 0;

  printf("Child%d doing simula2\n",nbrChld);
  fflush(stdout);

  for(int i = 0; i < 1000; i++){
    int rnd = (int) (rand() % (upper - lower + 1)) + lower;
    if (rnd == p) {
      return 0;
    }
  }
  return -1;
}

/*i. In that case, that 25 processes have finished and
 none of the searches have been done successfully.*/
void stopEveryThing(){
  printf("Inefficient algorithm!\n");
  fflush(stdout);
  for (int i = 0; i < 50 ; i++) {
    kill(allSons[i],SIGINT);
  }
}

/*ii. When one child process has done a successful search, the parent process
sends a signal to all the remaining running children processes to start the execution
of the simula2()*/
void newTask(){
  printf("Resume on Simula2\n");
  fflush(stdout);
  for (int i = 0; i < 50 ; i++) {
    kill(allSons[i],SIGCONT);
  }
}
