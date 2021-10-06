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

typedef struct
{
    char str[10][100];
    int qtdReader;
    int qtdWriter;
    int index;
} info;

int main()
{
    int fd;

    int data_size = sizeof(info);

    fd = shm_open("/infoReader", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

    if (fd == -1)
    {
        perror("Erro a abrir a memoria partilhada.");
        exit(1);
    }

    ftruncate(fd, data_size);

    info *shared = (info *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    fd = munmap(shared, data_size);
    if (fd < 0)
    {
        perror("Error at munmap()!!!\n");
        exit(1);
    }
    fd = close(fd);
    if (fd < 0)
    {
        perror("Erro a fechar!!!\n");
        exit(1);
    }
    fd = shm_unlink("/infoReader");
    if (fd < 0)
    {
        perror("Erro shm_unlink()!!!\n");
        exit(1);
    }

    return 0;
}