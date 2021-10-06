#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>
#include <ctype.h>

#define qtdSales 50000
#define qtdSons 10


typedef struct{
  int customer_code;
  int product_code;
  int quantity;
} Sale;

void fillArray(Sale *sales){
  int i;
  time_t t;

  srand ((unsigned) time (&t));

  for(i = 0; i < qtdSales; i++){
    sales[i].customer_code = i+1;
    sales[i].product_code = qtdSales + i;
    sales[i].quantity = (int) (rand() % (30 - 1 +1) + 1);
  }
}





int main(){
  int range = qtdSales / qtdSons;
  Sale sales[qtdSales];
  fillArray(sales);
  int j;
  pid_t pid;
  int status;
  int products[qtdSales];
  memset(products, 0, sizeof(products));
  int index = 0;
  int fd[2];

  if(pipe(fd) == -1){
    perror("Erro");
  }

  for(j = 0; j < qtdSons; j++){

    int start = j * range;
    int final = start + range;

    pid = fork();

    if(pid == 0){
      close(fd[0]);
      int k = 0;
      for(k = start; k < final; k++){
        if(sales[k].quantity > 20){

          int prod_code = sales[k].product_code;
          write(fd[1],&prod_code, sizeof(prod_code));
        }
      }
      close(fd[1]);
      exit(1);
    }

  }


  int read_msg;
  close(fd[1]);

  while(read(fd[0], &read_msg, sizeof(read_msg)) > 0){
    products[index] = read_msg;
    index++;
  }


  while ((pid = wait(&status)) > 0);
  close(fd[0]);
  index = 0;

  while(products[index] > 0){
    printf("Product ID com quantidade > 20: %d\n", products[index]);
    index++;
  }





  return 0;
}
