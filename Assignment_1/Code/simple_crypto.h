#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

unsigned char* randomKey(int size){
    unsigned char *buffer = (unsigned char *)malloc(sizeof(unsigned char )*size);
    int fd = open("/dev/urandom", O_RDONLY);
    read(fd, buffer, size);
    close(fd);
    return buffer;
}

unsigned char* OTP_decrypt(unsigned char * str,unsigned char * key, int size){
    unsigned char *buffer = (unsigned char *)malloc(sizeof(unsigned char )*size);
    for (int i=0; i<strlen(str); i++)
        buffer[i] = str[i] ^ key[i];
    return buffer;
}


char* OTP_encrypt(char * str,char * key, int size){
    char *buffer = (unsigned char *)malloc(sizeof(unsigned char )*size);
    for (int i=0; i<strlen(str); i++)
        buffer[i] = str[i] ^ key[i];
    return buffer;
}