#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <errno.h>

    #define CHAR_MAX_NAME 30
    #define CHAR_MAX_ADDRESS 100
    #define MAX_RECORDS 100

    typedef struct {
        int number;
        char name[CHAR_MAX_NAME];
        char address[CHAR_MAX_ADDRESS];
    } personalData;

    typedef struct {
        personalData dataRecords[MAX_RECORDS];
    } personalDataRecords;

    /*
     * Menu UI para o utilizador interagir com o programa
     */
    int menu() {
        int opt;

        printf("1. Consult\n");
        printf("2. Insert\n");
        printf("3. Consult All\n");
        printf("0. Exit\n");

        scanf("%d", &opt);

        return opt;
    }

    /*
     * Permite correr diversos programas ao mesmo tempo, isto é o insert o consult e o consult_all
     */
    void runProgram(char *programa) {

        pid_t pid = fork();

        if (pid == 0) {
            execlp(programa, programa, (char *)NULL);
            exit(1);
        }
        while (wait(NULL) > 0) {
            // Espera que todos os filhos terminem
        };
    }


int main(int argc, char *agrv[]) {
    //___________________________________Memoria_Partilhada___________________________________________________________________//
    /*
     * Inicio da abertura da memória partilhada
     */
    int fd, semR;
    int data_size = sizeof(personalDataRecords);
    /*
     * Abertura da memória partilhada
     */
    fd = shm_open("/share_data", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Erro a abrir a memoria partilhada.");
        exit(1);
    }
    /*
     * Define tamanho da estrutura a passar para a memória partilhada
     */
    ftruncate(fd, data_size);
    /*
     * Obtem o pointer da estrutura da memoria partilhada
     */
    personalDataRecords *sharedData = (personalDataRecords *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    //_______________________________________Semaforo_________________________________________________________________________//
    /*
     * Inicio da criação do semáforo
     */
    sem_t *sem;
    /*
     * Cria e abre semaforo
     */
    if ((sem = sem_open("semaforo", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(1);
    }
    //___________________________________Inicia_Programa_____________________________________________________________________//
    char str[300];
    char margem[100];
    strcpy(str,"________________MAIN_PROGRAM_____________\n");
    strcpy(margem,"_________________________________________\n");
    puts(margem);
    puts(str);
    puts(margem);
    semR = sem_wait(sem);
    if ( semR == -1) {
       perror("Erro sem_wait()!");
       exit(EXIT_FAILURE);
    }

    int i;
    for(i = 0; i < MAX_RECORDS; i++) {
        sharedData->dataRecords[i].number = 0;
    }

    semR = sem_post(sem);
    if ( semR == -1) {
       perror("Erro sem_post()!");
       exit(EXIT_FAILURE);
    }

    int opt;

    do {
        opt = menu();

        switch (opt) {
            case 1:
                runProgram("./consult.o");
                break;
            case 2:
                runProgram("./insert.o");
                break;
            case 3:
                runProgram("./consult_all.o");
                break;
            case 0:
                return 0;
            default:
                printf("\nError on the option selected!!!");
                return 0;
        }
    } while(opt == 1 || opt == 2 || opt == 3);

    //___________________________________Fecha_Memoria_Partilhada___________________________________________________________________//
    fd = munmap(sharedData, data_size);
    if ( fd < 0) {
        perror("Error at munmap()!!!\n");
        exit(1);
    }
    fd = close(fd);
    if ( fd < 0) {
        perror("Erro a fechar!!!\n");
        exit(1);
    }
    fd = shm_unlink("/share_data");
    if ( fd < 0) {
       perror("Erro shm_unlink()!!!\n");
       exit(1);
    }
    //___________________________________Fecha_Semáforos_________________________________________________________________________//
    semR = sem_close(sem);
    if ( semR < 0) {
        perror("Erro sem_close()!\n");
        exit(1);
    }
    semR = sem_unlink("semaforo");
    if ( semR != 0) {
        int error = errno;
        printf("Error: %s", strerror(error));
        printf("Erro shm_unlink() do semaforo!\n");
        exit(1);
    }

    return 0;
}
