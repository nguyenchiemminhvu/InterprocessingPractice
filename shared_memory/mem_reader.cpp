#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <semaphore.h>
#include "shared_sem.h"

shared_locker locker;

void __attribute__((constructor)) CONSTRUCTOR()
{
    sem_init(&locker.level1, 1, 0);
    sem_init(&locker.level2, 1, 0);
}

void __attribute__((destructor)) DESTRUCTOR()
{
    sem_destroy(&locker.level1);
    sem_destroy(&locker.level2);
}

int main()
{
    int fd = shm_open("backing_file", O_RDWR, 0644);
    if (fd < 0)
    {
        printf("Failed to open shared memomy\n");
        return -1;
    }

    caddr_t shared_mem_ptr = (char*)mmap(NULL, 1024, PROT_READ, MAP_SHARED, fd, 0);
    if (shared_mem_ptr == (char*)-1)
    {
        printf("Failed to get the segment\n");
        return -2;
    }

    for (int i = 0; i < 1024; i++)
    {
        write(STDOUT_FILENO, &shared_mem_ptr[i], 1);
    }

    munmap(shared_mem_ptr, 1024);

    close(fd);

    shm_unlink("./backing_file");

    return 0;
}