#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>

#define SAMPLE_FILE "sample_file"

int main()
{
    struct flock fl;
    fl.l_type = F_RDLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;
    fl.l_pid = getpid();

    int fd = open(SAMPLE_FILE, O_RDONLY);
    if (fd == -1)
    {
        printf("Failed to open sample file\n");
        exit(-1);
    }

    int rc = 0;
    rc = fcntl(fd, F_GETLK, &fl);
    if (fl.l_type != F_UNLCK)
    {
        printf("Failed to get lock\n");
        exit(-1);
    }

    //sleep(5);

    char buffer[255] = { 0 };
    while (rc = read(fd, buffer, 255) != 0)
    {
        write(STDOUT_FILENO, buffer, strlen(buffer));
        write(STDOUT_FILENO, "\n", 1);
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