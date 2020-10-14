#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>

unsigned char *randomKey(int size)
{
    unsigned char *buffer = (unsigned char *)malloc(sizeof(unsigned char) * size);
    int fd = open("/dev/urandom", O_RDONLY);
    read(fd, buffer, size);
    close(fd);
    return buffer;
}

unsigned char *OTP_decrypt(unsigned char *str, unsigned char *key, int size)
{
    unsigned char *buffer = (unsigned char *)malloc(sizeof(unsigned char) * size);
    for (int i = 0; i < size; i++)
        buffer[i] = str[i] ^ key[i];
    return buffer;
}

unsigned char *OTP_encrypt(char *str, char *key, int size)
{
    unsigned char *buffer = (unsigned char *)malloc(sizeof(unsigned char) * size);
    for (int i = 0; i < size; i++)
        buffer[i] = str[i] ^ key[i];
    return buffer;
}

char printCharacter(char ch)
{
    if (isprint(ch))
        return ch;
    else
    {
        switch (ch)
        {
        case '\n':
            return '\n';
        default:
            return '*';
        }
    }
}