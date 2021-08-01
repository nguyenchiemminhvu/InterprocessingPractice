#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>

#define SAMPLE_FILE "sample_file"

int main()
{
    fork();
    struct flock fl;
    fl.l_type = F_WRLCK; // lock write action on file
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;
    fl.l_pid = getpid();

    int fd = open(SAMPLE_FILE, O_RDWR | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        printf("Failed to open sample file\n");
        exit(-1);
    }

    int rc = fcntl(fd, F_SETLKW, &fl); // use F_SETLK if user want to return immediately when failed to get lock
    if (rc == -1)
    {
        printf("Failed to get lock\n");
        exit(-1);
    }

    sleep(5);

    char * buffer = "In the beginning God created the heaven and the earth\n";
    rc = write(fd, buffer, strlen(buffer));
    
    if (rc == -1)
    {
        printf("Failed to write data to sample file\n");
    }
    
    fl.l_type = F_UNLCK;
    rc = fcntl(fd, F_SETLK, &fl);
    if (rc == -1)
    {
        printf("Failed to release lock\n");
        exit(-1);
    }

    close(fd);

    return 0;
}