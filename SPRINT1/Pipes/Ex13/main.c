#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define NUMBER_OF_CHILDREN 4
#define NUMBER_OF_PIECES 1000

/*estrutura para partilhar informação com as máquinas*/
typedef struct {
 int quantity;
 int notification;
} transfer;

int spawn_childs(int nbrChilds){
    pid_t pid;
    int i;
    for (i = 0; i < nbrChilds; i++){
        pid = fork();
        if (pid < 0) {
          return -1;
        } else if (pid == 0) {
          return i + 1;
        }
    }
    return 0;
}

void cut(int n, int cutted){
  printf("-Cutting %d missing %d...\n", n,  NUMBER_OF_PIECES-cutted);
}

void fold(int n, int folded){
  printf("---Folding %d missing %d...\n", n,  NUMBER_OF_PIECES-folded);
}

void weld(int n, int welded){
  printf("------Welding %d missing %d...\n", n,  NUMBER_OF_PIECES-welded);
}

void pack(int n, int packed){
  printf("----------Packing and transfering %d missing %d...\n", n,  NUMBER_OF_PIECES-packed);
}

void receive(int n, int total){
  printf("---------------A1 is receiving %d information and has a total of %d pieces\n", n, total);
}


int main(){
  int i = 0;
  int fd[NUMBER_OF_CHILDREN+1][2];
  int M3ToM1 = NUMBER_OF_CHILDREN;
  int M4ToM1 = NUMBER_OF_CHILDREN+1;

  for(i = 0; i < NUMBER_OF_CHILDREN; i++){
    if(pipe(fd[i]) == -1){
      perror("Erro no pipe");
      return 1;
    }
  }

  int id = spawn_childs(NUMBER_OF_CHILDREN);

  switch(id)
  {
    case 0:
      close(fd[id][1]);

      int stored = 0;
      transfer a1 = {0,-1};

      do {
        read(fd[id][0], &a1, sizeof(a1));
        stored+=a1.quantity;
        receive(a1.quantity, stored);
      } while(a1.notification != 0);

      close(fd[id][0]);

    break;

    case 1:
    close(fd[id][0]);
    close(fd[M3ToM1][1]);
    close(fd[M4ToM1][1]);

    int cnt = 0;
    transfer m1 = {0,-1};
    int signal = 0;

    do {
      if (signal != 0) {
        read(fd[M3ToM1][0], &signal, sizeof(signal));
      }
      signal = 0;
      if (signal != 0) {
        read(fd[M4ToM1][0], &signal, sizeof(signal));
      }
      signal = 0;
      m1.quantity = 5;
      cnt+=m1.quantity;
      cut(m1.quantity,cnt);
      if (cnt == NUMBER_OF_PIECES) {
        m1.notification = 0;
        write(fd[id][1], &m1, sizeof(m1));
      } else {
        m1.notification = 1;
        write(fd[id][1], &m1, sizeof(m1));
      }

    } while(cnt != NUMBER_OF_PIECES);

    close(fd[M4ToM1][0]);
    close(fd[M3ToM1][0]);
    close(fd[id][1]);
    exit(1);

    break;

    case 2:
    close(fd[id-1][1]);
    close(fd[id][0]);

    int cnt2 = 0;
    transfer m2 = {0,-1};

    do {
      read(fd[id-1][0], &m2, sizeof(m2));
      cnt2 += m2.quantity;
      fold(m2.quantity,cnt2);
      m2.quantity = 5;
      write(fd[id][1], &m2, sizeof(  m2));
    } while(m2.notification != 0);

    close(fd[id][1]);
    close(fd[id-1][0]);
    exit(1);

    break;

    case 3:
    close(fd[M3ToM1][0]);
    close(fd[id-1][1]);
    close(fd[id][0]);

    int cnt3 = 0;
    transfer m3 = {0,-1};
    int repeat = 1;

    do {
      int incr = 0;
      do {
          read(fd[id-1][0], &m3, sizeof(m3));
          incr += m3.quantity;
          write(fd[M3ToM1][1], &repeat, sizeof(repeat));
      } while(incr != 10);

      m3.quantity = 10;
      cnt3 += m3.quantity;
      weld( m3.quantity,cnt3);
      write(fd[id][1], &m3, sizeof(m3));
    } while(m3.notification != 0);

    close(fd[id][1]);
    close(fd[id-1][0]);
    close(fd[M3ToM1][1]);
    exit(1);

    break;

    case 4:
    close(fd[M4ToM1][0]);
    close(fd[id-1][1]);
    close(fd[0][0]);

    int cnt4 = 0;
    transfer m4 = {0,-1};
    int repeat2 = 1;

    do {
      int incr2 = 0;
      do {
          read(fd[id-1][0], &m4, sizeof(m4));
          incr2 += m4.quantity;
          write(fd[M4ToM1][1], &repeat2, sizeof(repeat2));
      } while(incr2 != 100);

      m4.quantity = 100;
      cnt4 += m4.quantity;
      pack(m4.quantity,cnt4);
      write(fd[0][1], &m4, sizeof(m4));
    } while(m4.notification != 0);

    close(fd[0][1]);
    close(fd[id-1][0]);
    close(fd[M4ToM1][1]);
    exit(1);

    break;
  }
  
  return 0;
}
