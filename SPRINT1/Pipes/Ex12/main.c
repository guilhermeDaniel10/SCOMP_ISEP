#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/*Quantidade de barcodeReaders*/
#define NUMBER_OF_CHILDREN 5


/*Array of pipes*/
int arrayOfPipes[NUMBER_OF_CHILDREN+1][2];

/*estrutura para partilhar informação com a baseDados "Pai"*/
typedef struct {
 int refOfProduct;
 int pipeInfo;
} barcodeReader;

/*informaçao do produto*/
typedef struct {
 int refOfProduct;
 float price;
 char description[30];
} productInfo;

int main(void) {
    int i;
    pid_t pid;

    for (i = 0; i < NUMBER_OF_CHILDREN+1; i++) {
        if (pipe(arrayOfPipes[i]) == -1){
            perror("Erro no Pipe");
            return 1;
        }
    }
    char arr[10];

    printf("\nInsira a referencia do produto:");
    for (i = 0; i < NUMBER_OF_CHILDREN && fgets(arr, 10, stdin) != NULL; i++) {

      pid = fork();

      if(pid == -1){
  			perror("Erro no fork");
  			return 1;
		  }

      if(pid == 0){
        /*barcodeReader runs while shutDown is diferent of -1*/
        //int shutDown = 0;
        /*referencia introduzida pelo utilizador*/
        int refDoProduto;
        /*Informação enviada para o pai*/
        barcodeReader info;
        /*Informação recebida pelo pai sobre o produto pedido*/
        productInfo infoAsked;
        /* fecha a extremidade não usada, filho só escreve no pipe partilhado entre todos os filhos */
        close(arrayOfPipes[0][0]);
        /* fecha a extremidade não usada, filho só recebe info do pai */
        close(arrayOfPipes[i+1][1]);


        /*printf("Deseja utilizar o Barcode Reader da máquina %d:\n Insira 1, se de facto o vai usar...\n Insira 0, se não o pretende utilizar...\n->",i+1);
        scanf("%d\n", &shutDown);


          switch(shutDown) {
                case 1:
                    printf("\nInsira a referencia do produto:");
                    scanf(" %d", &refDoProduto);*/
                    refDoProduto = atoi(arr);
                    info.refOfProduct = refDoProduto;
                    info.pipeInfo = i;
                    write(arrayOfPipes[0][1], &info, sizeof(info));
                    read(arrayOfPipes[i+1][0], &infoAsked, sizeof(infoAsked));
                    if (infoAsked.price == -1.0) {
                      printf("O produto com a referencia %d não está presente no sistema;\n", infoAsked.refOfProduct);
                    } else {
                      printf("O produto com a referencia %d custa %.2f possui a descrição '%s';\n", infoAsked.refOfProduct ,infoAsked.price ,infoAsked.description);
                    }
                    /*break;

                case 0:
                    printf("\nProxima máquina!\n");
                    break;

                default:
                    printf("\nOpção inesistente!\n");
            }*/
            if (i == NUMBER_OF_CHILDREN-1) {
              /* fecha a extremidade que já não vai ser usada, partilhado entre todos os filhos */
              close(arrayOfPipes[0][1]);
            }
            /* fecha a extremidade que já não é mais usada */
            close(arrayOfPipes[i+1][0]);
            exit(i);
          }
          if(i != NUMBER_OF_CHILDREN - 1){
            printf("\nInsira a referencia do produto:");
          }

    }

    /* fecha a extremidade não usada pai só lê pipe partilhado com os 5 filhos */
    close(arrayOfPipes[0][1]);
    /*informações das máquinas*/
    barcodeReader infoBarcodeReader;
    /*dados do protuto pedido pelo filho*/
    productInfo foundProduct;
    /*Base de Dados que só o PAI possui*/
    productInfo baseDados[10] = {
      {1171589,21.7,"Frango KG"},
      {1181743,15.3,"Peixe KG"},
      {1111111,1.5,"Pão com manteiga"},
      {2222222,4.9,"Chocolate"},
      {3333333,1.99,"Manteiga"},
      {4444444,3.45,"Vinho Tinto"},
      {5555555,3.99,"Vinho Verde"},
      {6666666,1,"Tomate Cereja"},
      {7777777,3.99,"Cereais Chocapic"},
      {8888888,2.45,"Compota"}  };

      for(i = 0; i < NUMBER_OF_CHILDREN; i++){
          /*fecha a extremidade não usada pai só escreve para filho em específico */
          close(arrayOfPipes[i+1][0]);
          /*Inicializar estrutura*/
          read(arrayOfPipes[0][0], &infoBarcodeReader, sizeof(infoBarcodeReader));
          /*posição do array do pipe certo*/
          //int pipeIncr = infoBarcodeReader.pipeInfo;
          /*Encontra uma ref na base de dados*/
          int ref = infoBarcodeReader.refOfProduct;
          productInfo notFoundProduct = {-1,-1.0,"NaN"};
          int flagFound = -1;

            for (int j = 0; j < 10; j++) {
                foundProduct = baseDados[j];
                  if (foundProduct.refOfProduct == ref) {
                      flagFound = 0;
                      /*envia informaçao do produto ao filho que a pediu*/
                      write(arrayOfPipes[i+1][1], &foundProduct, sizeof(foundProduct));
                      break;
                  }
             }

            if (flagFound == -1) {
              notFoundProduct.refOfProduct = ref;
              write(arrayOfPipes[i+1][1], &notFoundProduct, sizeof(notFoundProduct));
            }

            if (NUMBER_OF_CHILDREN-1 == i) {
              /* fecha a extremidade que já não vai ser usada */
              close(arrayOfPipes[0][0]);
            }
            /* fecha a extremidade que já não vai ser usada */
            close(arrayOfPipes[i+1][1]);
        }

    return 0;
}
