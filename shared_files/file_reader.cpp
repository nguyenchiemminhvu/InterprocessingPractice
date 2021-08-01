#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>

#define SAMPLE_FILE "sample_file"

int main()
{
    int fd = open(SAMPLE_FILE, O_RDONLY);
    if (fd == -1)
    {
        printf("Failed to open sample file\n");
        exit(-1);
    }
    int rc = 0;

    char buffer[255] = { 0 };
    while (rc = read(fd, buffer, 255) != 0)
    {
        write(STDOUT_FILENO, buffer, strlen(buffer));
        write(STDOUT_FILENO, "\n", 1);
    }

    close(fd);

    return 0;
}