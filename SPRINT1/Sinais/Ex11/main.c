#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>




/*
  Estrutura do comando
*/
typedef struct{
    char cmd[32];
    int tempo;
} comando;

char currentCmd[32];
#define numeroComandos 5
comando comandos[5];



char matriz[numeroComandos][20];
int matrizPids[numeroComandos];
/*
Handle do alarme
*/
void handle_alarm(int signo){
  int pid = getpid();
  char str[120];
  int k = 0;

  for(k = 0; k < numeroComandos; k++){
    if(matrizPids[k] == pid){
      sprintf(str, "The command %s didn't end in its allowed time!\n", matriz[k]);

    }
  }


  write(STDOUT_FILENO, str, strlen(str));
  kill(pid,SIGKILL);
}

void copyTomatriz(int pos, char *cmdAtual){
  strcpy(matriz[pos], cmdAtual);
}


/*
  Handle para o sinal do child
*/
void handle_child(int signo, siginfo_t *sinfo, void *context){
  char *str = "Execution ended.\n";
  write(STDOUT_FILENO, str, strlen(str));

}

/*
  Parte um comando em dois, retornando apenas a parte mais à direita
*/
char* scdCmd(char* cmd, int i){
  char *token;
  char *search = " ";

  token = strtok(cmd, search);
  token = strtok(NULL, search);

  return token;
}

/*
  Verifica se um comando tem espaço
*/
int hasSpace(char *s){
  int bol = 0;
  int j;

  for(j = 0; j < strlen(s); j++){
    if(s[j] == ' '){
      bol = 1;
    }
  }
  return bol;
}

/*
  Remove o espaço de um comando
*/
char* removeSpace(char *s){
  int i = 0, j = 0;
  while(s[i]){
    if(s[i] != ' ')
      s[j++] = s[i];
    i++;
  }
  s[j] = '\0';
  return s;
}

/*
  Verifica se um comando tem duas palavras
*/
int oneWord(char *s){
  int cnt = 0;
  int found = 0;
  int i = 0;
  for(i = 0; i < strlen(s); i++){
    if(found != 0 && s[i] != ' '){
      cnt++;

    }
    if(s[i] == ' '){
      found = 1;
    }
  }
  return cnt;
}


/*
  Verifica se o comando é um ficheiro executavel de c
*/
int isCProgram(char *s){
  if(strstr(s, ".c") != NULL){
    return 1;
  }
  return 0;
}

/*
  Remove o .c do ficheiro a executar
*/
char* removeC(char *s){
  int i = 0, j = 0;
  while(s[i]){
    if(s[i] != '.' && s[i] != 'c')
      s[j++] = s[i];
    i++;
  }
  s[j] = '\0';
  return s;
}

int main(){
  pid_t pid;

  int i = 0;

  strcpy(comandos[0].cmd, "ls");
  comandos[0].tempo = 1;

  strcpy(comandos[1].cmd, "pwd");
  comandos[1].tempo = 1;

  strcpy(comandos[2].cmd, "prog1.c");
  comandos[2].tempo = 1;

  strcpy(comandos[3].cmd, "prog2.c");
  comandos[3].tempo = 11;

  strcpy(comandos[4].cmd, "ls -l");
  comandos[4].tempo = 1;


  /* Tratamento do ealarme SIGCHILD */
  struct sigaction act;
  memset(&act, 0, sizeof(struct sigaction));
  sigfillset(&act.sa_mask);
  act.sa_sigaction = handle_child;
  act.sa_flags = SA_SIGINFO;
  sigaction(SIGCHLD, &act, NULL);


  /* Tratamento do ealarme SIGALARM */
  memset(&act, 0, sizeof(struct sigaction));
  sigemptyset(&act.sa_mask);
  act.sa_handler = handle_alarm;
  sigaction(SIGALRM, &act, NULL);
  int status;


  for(i = 0; i < numeroComandos; i++){


    pid = fork();

    if(pid == 0){
      matrizPids[i] = getpid();
      copyTomatriz(i, comandos[i].cmd);



      /* Verifica se o comando é um ficheiro c*/
      if(isCProgram(comandos[i].cmd) == 1){
        char *prog = removeC(comandos[i].cmd);
        char str[120];
        sprintf(str, "./%s", prog);
        alarm(comandos[i].tempo);

        execlp(str, prog, (char*)NULL);
        exit(-1);
      } else {

      /* Verifica se o comando não tem espaço */
      if(hasSpace(comandos[i].cmd) == 0){
        alarm(comandos[i].tempo);
        //Teste para dar errado
        sleep(3);

        execlp(comandos[i].cmd, comandos[i].cmd, (char*)NULL);
        exit(-1);
    }else {

      /* Verifica se o comando não tem apenas uma palavra tendo espaço */
      if(oneWord(comandos[i].cmd) == 0){
        char *noSpace = removeSpace(comandos[i].cmd);

        alarm(comandos[i].tempo);

        execlp(noSpace, noSpace, (char*)NULL);
        exit(-1);

      /* Verifica se o comando tem apenas uma palavra tendo espaço */
      } else if(oneWord(comandos[i].cmd) > 0) {

        char *flag = scdCmd(comandos[i].cmd, i);
        char *noSpace = removeSpace(comandos[i].cmd);

        alarm(comandos[i].tempo);

        execlp(noSpace, noSpace, flag, (char*)NULL);
        exit(-1);

        }
      }
    }
  }
}

while((pid = wait(&status)) > 0);
}
