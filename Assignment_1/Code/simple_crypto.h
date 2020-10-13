#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

unsigned char*  randomKey(int size){
    unsigned char *buffer = (unsigned char *)malloc(sizeof(unsigned char )*size);
    int fd = open("/dev/urandom", O_RDONLY);
    read(fd, buffer, size);
    close(fd);
    return buffer;
}
