#include "simple_crypto.h"

// ###############################         GENERAL USE FUNCTIONS              ###############################

bool isInRange(int lowerLimit, int upperLimit, int no)
{
    return (lowerLimit <= no && no <= upperLimit);
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

// ###############################         OTP ALGORITHM FUNCTIONS           ###############################


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
    // allocate memory for decrypted text
    unsigned char *buffer = (unsigned char *)malloc(sizeof(unsigned char) * size);
    // XOR-ing the encrypted text with the key
    for (int i = 0; i < size; i++)
        buffer[i] = str[i] ^ key[i];
    return buffer;
}

unsigned char *OTP_encrypt(char *str, char *key, int size)
{
    // allocate memory for encrypted text
    unsigned char *buffer = (unsigned char *)malloc(sizeof(unsigned char) * size);
    // XOR-ing the text with the key to get the encrypted text
    for (int i = 0; i < size; i++)
        buffer[i] = str[i] ^ key[i];
    return buffer;
}

// ###############################         CAESAR'S ALGORITHM FUNCTIONS       ###############################

enum category findCharacterCategory(char ch)
{
    int charAscii = (int)ch;
    if (isInRange(numStart, numEnd, charAscii))
        return number;
    else if (isInRange(capitalStart, capitalEnd, charAscii))
        return capital;
    else if (isInRange(lowerStart, lowerEnd, charAscii))
        return lower;
}

char shiftCharacter(char ch, int shiftIndicator, int categoryStart, int categoryEnd, int nextCategoriesSplitter,
                    int changeCategoryLimit, int firstGap, int secondGap)
{
    int charAscii = (int)ch;
    int returnAscii;
    //    increment in the same category
    if (charAscii + shiftIndicator <= categoryEnd)
        returnAscii = charAscii + shiftIndicator;
    //    increment in one of the next category
    else if (shiftIndicator - (categoryEnd - charAscii) <= changeCategoryLimit)
    {
        int untilCatEnd = categoryEnd - charAscii;
        //        increment in the next category
        if (shiftIndicator - untilCatEnd <= nextCategoriesSplitter)
        {
            returnAscii = charAscii + firstGap + shiftIndicator;
        }
        //        increment in the second category
        else
            returnAscii = charAscii + firstGap + secondGap + shiftIndicator;
    }
    else
    {
        int inCatChange = shiftIndicator - changeCategoryLimit - (categoryEnd - charAscii);
        returnAscii = categoryStart + inCatChange - 1;
    }
    return (char)returnAscii;
}

unsigned char *CS_encrypt(char *str, int shiftIndicator, int size)
{
    // allocate memory for encrypted text
    unsigned char *buffer = (unsigned char *)malloc(sizeof(unsigned char) * size);
    shiftIndicator = shiftIndicator % 62;

    for (int i = 0; i < size; i++)
    {
        int categoryEnd, categoryStart, changeCategoryLimit, nextCategoriesSplitter, firstGap, secondGap;
        char currentChar = str[i];

        switch (findCharacterCategory(currentChar))
        {
        case number:
            categoryStart = numStart;
            categoryEnd = numEnd;
            changeCategoryLimit = 52;
            nextCategoriesSplitter = 26;
            firstGap = numberToCapital;
            secondGap = capitalToLower;
            break;
        case capital:
            categoryStart = capitalStart;
            categoryEnd = capitalEnd;
            changeCategoryLimit = 36;
            nextCategoriesSplitter = 26;
            firstGap = capitalToLower;
            secondGap = lowerToNumber;
            break;
        case lower:
            categoryStart = lowerStart;
            categoryEnd = lowerEnd;
            changeCategoryLimit = 36;
            nextCategoriesSplitter = 10;
            firstGap = lowerToNumber;
            secondGap = numberToCapital;
            break;
        default:
            printf("Non-acceptable character");
            break;
        }
        buffer[i] = shiftCharacter(currentChar, shiftIndicator, categoryStart, categoryEnd, nextCategoriesSplitter,
                                   changeCategoryLimit, firstGap, secondGap);
    }
    return buffer;
}


unsigned char *CS_decrypt(unsigned char *str, int shiftIndicator, int size)
{
    shiftIndicator = shiftIndicator % 62;
    shiftIndicator = encryptingCharacters - shiftIndicator;
    return CS_encrypt(str, shiftIndicator, size);
}