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
    int fd = shm_open("backing_file", O_RDWR | O_CREAT, 0644);
    if (fd < 0)
    {
        printf("Failed to open shared memomy\n");
        return -1;
    }

    ftruncate(fd, 1024);

    caddr_t shared_mem_ptr = (char*)mmap(NULL, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared_mem_ptr == (char*) -1)
    {
        printf("Failed to get the segment\n");
        return -2;
    }

    strcpy(shared_mem_ptr, "God is good all the time!\n");

    munmap(shared_mem_ptr, 1024);

    close(fd);

    shm_unlink("./backing_file");

    return 0;
}