#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <semaphore.h>

sem_t *locker_1;
sem_t *locker_2;

int main()
{
    int fd = shm_open("backing", O_RDWR, 0644);
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
    
    locker_1 = sem_open("locker_1", O_CREAT, 0644, 0);
    locker_2 = sem_open("locker_2", O_CREAT, 0644, 0);

    sem_wait(locker_1);

    for (int i = 0; i < 1024; i++)
    {
        write(STDOUT_FILENO, &shared_mem_ptr[i], 1);
    }

    sleep(5);
    
    sem_post(locker_2);

    munmap(shared_mem_ptr, 1024);

    close(fd);

    shm_unlink("backing");

    return 0;
}