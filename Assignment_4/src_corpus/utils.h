#ifndef _UTILS_H
#define _UTILS_H


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define DELIM ":"

char * getLineInfo(char * str);

char *trimwhitespace(char *str);

void print_hex(unsigned char *data, size_t len);

char *stringToHex(unsigned char *data, size_t len);

void print_string(unsigned char *data, size_t len);

#endif /* _UTILS_H */
