#include "simple_crypto.h"

// ###############################         GENERAL USE FUNCTIONS              ###############################

bool isInRange(int lowerLimit, int upperLimit, int no) {
    return (lowerLimit <= no && no <= upperLimit);
}

char printCharacter(char ch) {
    if (isprint(ch))
        return ch;
    else {
        switch (ch) {
            case '\n':
                return '\n';
            default:
                return '*';
        }
    }
}


char *readInput(FILE* fp, size_t size){
    char *str;
    int ch;
    size_t len = 0;
    str = realloc(NULL, sizeof(char)*size);
    if(!str){
        printf("%s" , str);
        return str;
    }
    //until get the \0 ,the termination of a character string, or the newline one
    while(EOF!=(ch=fgetc(fp)) && ch != '\n'){
        str[len++]=ch;
        //when the string reaches the allocated space, we reallocate space with 16 more.
        if(len==size){
            str = realloc(str, sizeof(char)*(size+=16));
            if(!str){
                printf("%s" , str);
                return str;
            }
        }
    }
    //add the terminal character
    str[len++]='\0';

    return realloc(str, sizeof(char)*len);
}

// ###############################         OTP ALGORITHM FUNCTIONS           ###############################

unsigned char *randomKey(int size) {
    unsigned char *buffer = (unsigned char *) malloc(sizeof(unsigned char) * size);
    int fd = open("/dev/urandom", O_RDONLY);
    read(fd, buffer, size);
    close(fd);
    return buffer;
}

unsigned char *OTP_decrypt(unsigned char *str, unsigned char *key, int size) {
    // allocate memory for decrypted text
    unsigned char *buffer = (unsigned char *) malloc(sizeof(unsigned char) * size);
    // XOR-ing the encrypted text with the key
    for (int i = 0; i < size; i++)
        buffer[i] = str[i] ^ key[i];
    return buffer;
}

unsigned char *OTP_encrypt(char *str, char *key, int size) {
    // allocate memory for encrypted text
    unsigned char *buffer = (unsigned char *) malloc(sizeof(unsigned char) * size);
    // XOR-ing the text with the key to get the encrypted text
    for (int i = 0; i < size; i++)
        buffer[i] = str[i] ^ key[i];
    return buffer;
}

// ###############################         CAESAR'S ALGORITHM FUNCTIONS       ###############################

enum category findCharacterCategory(char ch) {
    int charAscii = (int) ch;
    // check if character belong to number ascii codes
    if (isInRange(numStart, numEnd, charAscii))
        return number;
     // check if character belong to upper case letters ascii codes
    else if (isInRange(capitalStart, capitalEnd, charAscii))
        return capital;
     // check if character belong to lower case letters ascii codes
    else if (isInRange(lowerStart, lowerEnd, charAscii))
        return lower;
}

char shiftCharacter(char ch, int shiftIndicator, int categoryStart, int categoryEnd, int nextCategoriesSplitter,
                    int changeCategoryLimit, int firstGap, int secondGap) {
    int charAscii = (int) ch;
    int returnAscii;
    //    increment in the same category
    if (charAscii + shiftIndicator <= categoryEnd)
        returnAscii = charAscii + shiftIndicator;
        //    increment in one of the next category
    else if (shiftIndicator - (categoryEnd - charAscii) <= changeCategoryLimit) {
        int untilCatEnd = categoryEnd - charAscii;
        //        increment in the next category
        if (shiftIndicator - untilCatEnd <= nextCategoriesSplitter) {
            returnAscii = charAscii + firstGap + shiftIndicator;
        }
            //        increment in the second category
        else
            returnAscii = charAscii + firstGap + secondGap + shiftIndicator;
    } else {
        // increment in the same category when the shifting is more than the other two categories total length
        int inCatChange = shiftIndicator - changeCategoryLimit - (categoryEnd - charAscii);
        returnAscii = categoryStart + inCatChange - 1;
    }
    return (char) returnAscii;
}

unsigned char *CS_encrypt(char *str, int shiftIndicator, int size) {
    // allocate memory for encrypted text
    unsigned char *buffer = (unsigned char *) malloc(sizeof(unsigned char) * size);
    // get the modulo of the shifting indicator because its like we have a cyclic shifting
    shiftIndicator = shiftIndicator % encryptingCharacters;

    for (int i = 0; i < size; i++) {
        int categoryEnd, categoryStart, changeCategoryLimit, nextCategoriesSplitter, firstGap, secondGap;
        char currentChar = str[i];

        //init the parameters of the shiftCharacter function depend on the character category we have
        switch (findCharacterCategory(currentChar)) {
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

unsigned char *CS_decrypt(unsigned char *str, int shiftIndicator, int size) {
    // the decrypt function using the encrypt function given the complementary shift indicator as shift indicator
    shiftIndicator = shiftIndicator % encryptingCharacters;
    shiftIndicator = encryptingCharacters - shiftIndicator;
    return CS_encrypt(str, shiftIndicator, size);
}

// ###############################         VIGENERE'S ALGORITHM FUNCTIONS     ###############################

char shiftViaTabulaRecta(char column, char row){
    int columnAscii = (int) column;
    int rowAscii = (int) row;
    int shifting = rowAscii- capitalStart;
    int tillLastChar = capitalEnd - columnAscii;
    if (shifting <= tillLastChar ){
        return (char) (columnAscii + shifting );
    }
    else{
        return (char) (capitalStart + shifting - tillLastChar -1);
    }
}

unsigned char *VG_encrypt(char *str, char *key) {
    unsigned char *buffer = (unsigned char *) malloc(sizeof(unsigned char) * strlen(str));
    for (int i = 0; i < strlen(str); i++) {
        buffer[i] = shiftViaTabulaRecta((char) str[i],key[i % strlen(key)]);
    }
    return buffer;
}

char findColumnTabulaRecta(char depth, char row){
    int depthAscii = (int) depth;
    int rowAscii = (int) row;
    int tillLastChar = capitalEnd - rowAscii;
    int colShifting;
    colShifting = (rowAscii <= depthAscii ) ? (depthAscii - rowAscii) : (tillLastChar + (depthAscii - capitalStart + 1));
    return (char) (capitalStart + colShifting);
}


unsigned char *VG_decrypt(unsigned char *str, char *key) {
    unsigned char *buffer = (unsigned char *) malloc(sizeof(unsigned char) * strlen(str));
    for (int i = 0; i < strlen(str); i++) {
        buffer[i] = findColumnTabulaRecta((char) str[i],key[i % strlen(key)]);
    }
    return buffer;
}