#include <stdio.h>
#include <signal.h>

int main(void) {

 pid_t pid; /*process id of the process that we want to work with*/

 printf("Please insert the pid of the process in question:\n");
 scanf("%d", &pid);

 kill(pid,SIGUSR1);
 kill(pid,SIGUSR2);
}
