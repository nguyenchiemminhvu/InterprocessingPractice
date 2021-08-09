#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <shmem/shmem.h>
#include <semaphore.h>
#include "shared_sem.h"

int main()
{
    int fd = shm_open(BackingFile, O_RDWR, AccessPerms);
    if (fd < 0)
    {
        printf("Failed to open shared memomy\n");
        return -1;
    }

    caddr_t shared_mem_ptr = (char*)mmap(NULL, ByteSize, PROT_READ, MAP_SHARED, fd, 0);
    if (shared_mem_ptr == (char*)-1)
    {
        printf("Failed to get the segment\n");
        return -2;
    }

    for (int i = 0; i < ByteSize; i++)
    {
        write(STDOUT_FILENO, &shared_mem_ptr[i], 1);
    }

    munmap(shared_mem_ptr, ByteSize);

    close(fd);

    shm_unlink(BackingFile);

    return 0;
}