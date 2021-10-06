#include <stdio.h>
#include <signal.h>

int main(void) {

 pid_t pid; /*process id of the process that we want to work with*/
 int signal;/* signal that we want to run */

 printf("Please insert the pid of the process in question:\n");
 scanf("%d", &pid);
 do {
   printf("\nPlease insert the signal that we want to run:\n");
   scanf("%d", &signal);
   if (signal<1 || signal>64) {
      printf("The signal introduced doesnt exist!!!\n");
   }
 } while(signal<1 || signal>64);

 kill(pid,signal);

}
