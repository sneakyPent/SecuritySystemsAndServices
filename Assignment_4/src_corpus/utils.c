#include "utils.h"

char *getLineInfo(char *str)
{
    char *ptr = strtok(str, DELIM);
    ptr = strtok(NULL, DELIM);
    return ptr;
}

char *trimwhitespace(char *str)
{
    char *end;

    // Trim leading space
    while (isspace((unsigned char)*str))
        str++;

    if (*str == 0) // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;

    // Write new null terminator character
    end[1] = '\0';

    return str;
}

char *stringToHex(unsigned char *data, size_t len)
{
    size_t i;
    char *str = malloc(2 * len);

    if (!data)
        printf("NULL data\n");
    else
    {
        for (int i = 0, j = 0; i < len; ++i, j += 2)
            sprintf(str + j, "%02X", data[i] & 0xff);
        return str;
    }
}

void print_string(unsigned char *data, size_t len)
{
    size_t i;

    if (!data)
        printf("NULL data\n");
    else
    {
        for (i = 0; i < len; i++)
            printf("%c", data[i]);
        printf("\n");
    }
}