#include <stdio.h>
#include <string.h>
#include "utils.h"

void readFiles(char *fileName)
{
    int i;
    FILE *file;
    file = fopen(fileName, "r");
    if (file != NULL)
        fclose(file);
}

void writeFiles(char *fileName, char *message)
{

    int i;
    size_t bytes;
    FILE *file;
    file = fopen(fileName, "w+");
    if (file != NULL)
    {
        bytes = fwrite(message, strlen(message), 1, file);
        fclose(file);
    }
}

int main(int argc, char *argv[])
{
    int ch;
    char *fileName;
    char *message;
    while ((ch = getopt(argc, argv, "p:m:")) != -1)
    {
        switch (ch)
        {
        case 'p':
            fileName = optarg;
            break;
        case 'm':
            message = optarg;
            break;
        default:
            break;
        }
    }
    writeFiles(fileName, message);

    argc -= optind;
    argv += optind;
    return 0;
}
