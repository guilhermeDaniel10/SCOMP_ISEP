#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
  int i, parCounter = 0;
  /*int status;  variable not used*/
  pid_t p;
  int array[4];
  /*array = (int*) malloc(4*sizeof(int));*/
  /*
  for (i = 0; i < 4; i++) {
    if (fork() == 0) {
      sleep(1);
    }
  } variable not used*/

/*b)*/
  /*
  for (i = 0; i < 4; i++) {
    p = fork();
    if (p == 0) {
      printf("Child.\n");
      exit(1);
    }
  }
  */
/*c)*/
/*
  for (i = 0; i < 4; i++) {
    p = fork();
    if (p != 0) {
      if( ((int) p ) %2 ==0){
        printf("Filho pid par ==> c%d\n",(int) p);
        array[parCounter] = (int) p;
        parCounter++;
      }
    } else {
      printf("Child.\n");
      sleep(1);
      exit(1);
    }
  }

  if (p != 0) {
    printf("Wait\n");
    wait(array);
  }
*/

/*d)*/

for (i = 0; i < 4; i++) {
  p = fork();
  if (p != 0) {
    if( ((int) p ) %2 ==0){
      printf("Filho pid par ==> c%d\n",(int) p);
      array[parCounter] = (int) p;
      parCounter++;
    }
  } else {
    exit(i);
  }
}

if (p != 0) {
  printf("Wait\n");
  wait(array);
}

 printf("This is the end.\n");
}
