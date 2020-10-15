#ifndef SIMPLE_CRYPTO_H
#define SIMPLE_CRYPTO_H


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>

#define bool int
#define true 1
#define false 0

enum category
{
    number,
    capital,
    lower
};

#define encryptingCharacters 62
#define numStart 48
#define numEnd 57
#define capitalStart 65
#define capitalEnd 90
#define lowerStart 97
#define lowerEnd 122

#define numberToCapital 7
#define capitalToLower 6
#define lowerToNumber -75

// ###############################         GENERAL USE FUNCTIONS              ###############################

bool isInRange(int lowerLimit, int upperLimit, int no);

/**
    @brief: Fucntion to print "*" in case of non-printable character

    @param: The character about to get printed

    @return: The same character if its printable otherwise '*' character
*/
char printCharacter(char ch);

// ###############################         OTP ALGORITHM FUNCTIONS           ###############################

/**
    @brief: Function to get a random key of given size for the OTP algorithm

    @param: The encryption key size

    @return: The encryption key
*/
unsigned char *randomKey(int size);

/**
    @brief: Fucntion to decrypt the plaintext in OTP algorithm 

    @param: Encrypted plaintext, the encryption key and the user's input text size 

    @return: The decrypted plaintext
*/
unsigned char *OTP_decrypt(unsigned char *str, unsigned char *key, int size);

/**
    @brief: Fucntion to encrypt the given text in OTP algorithm 

    @param: User's input, the encryption key and the user's message size 

    @return: The encrypted plaintext
*/
unsigned char *OTP_encrypt(char *str, char *key, int size);

// ###############################         CAESAR'S ALGORITHM FUNCTIONS       ###############################

enum category findCharacterCategory(char ch);

char shiftCharacter(char ch, int shiftIndicator, int categoryStart, int categoryEnd, int nextCategoriesSplitter,
                    int changeCategoryLimit, int firstGap, int secondGap);

/**
    @brief: Fucntion to encrypt the given text in caesar's  algorithm 

    @param: User's input, the encryption key and the plaintext size 

    @return: The encrypted plaintext
*/
unsigned char *CS_encrypt(char *str, int shiftIndicator, int size);

/**
    @brief: Fucntion to decrypt the plaintext in caesar's algorithm 

    @param: The cipher-text, the positive shift indicator the uplaintext size 

    @return: The decrypted plaintext
*/
unsigned char *CS_decrypt(unsigned char *str, int shiftIndicator, int size);

// ###############################         VIGENERE'S ALGORITHM FUNCTIONS     ###############################

#endif //SIMPLE_CRYPTO_H