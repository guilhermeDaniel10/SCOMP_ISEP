#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void){
    pid_t p1;
    int status1;
    int status2;

    p1 = fork();
    if(p1 > 0){
      waitpid(p1,&status1,0);
      if(WIFEXITED(status1)){
        p1 = fork();
        if(p1 > 0){
          waitpid(p1,&status2,0);
            if(WIFEXITED(status2)){
              printf("\nchild 1 status = %d;\nchild 2 status = %d;\n",WEXITSTATUS(status1),WEXITSTATUS(status2));
            }
        }else{
          sleep(2);
          exit(2);
        }
      }
    } else{
      sleep(1);
      exit(1);
    }
}
