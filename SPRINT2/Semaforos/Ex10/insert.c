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
    fd = shm_open("/share_data", O_RDWR, S_IRUSR | S_IWUSR);
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
    if ((sem = sem_open("semaforo", 0)) == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(1);
    }
    //___________________________________Inicia_Programa_Consulta____________________________________________________________________//
    char str[300];
    char margem[100];
    strcpy(str,"_____________INSERT_RECORD______________\n");
    strcpy(margem,"_________________________________________\n");
    puts(margem);
    puts(str);
    puts(margem);

    int number,i;
    char name[CHAR_MAX_NAME];
    char address[CHAR_MAX_ADDRESS];

    printf("Insert number -> ");
    scanf("%d", &number);
    fflush(stdin);

    printf("Insert name -> ");
    scanf("%s", name);
    fflush(stdin);

    printf("Insert address -> ");
    scanf("%s", address);
    fflush(stdin);

    semR = sem_wait(sem);
    if ( semR == -1) {
       perror("Erro sem_wait()!");
       exit(EXIT_FAILURE);
    }

    for (i = 0; i < MAX_RECORDS; i++) {
			if (sharedData->dataRecords[i].number == 0) {
				sharedData->dataRecords[i].number = number;
				strcpy(sharedData->dataRecords[i].name, name);
				strcpy(sharedData->dataRecords[i].address, address);
				break;
			}
		}

    semR = sem_post(sem);
    if ( semR == -1) {
       perror("Erro sem_post()!");
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
