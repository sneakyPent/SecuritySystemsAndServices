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
    @function printCharacter

    \brief Fucntion to print "*" in case of non-printable character.

    @param ch The character about to get printed.

    @return The same character if its printable otherwise '*' character.
*/
char printCharacter(char ch);

// ###############################         OTP ALGORITHM FUNCTIONS           ###############################

/**
    \brief Function to get a random key of given size for the OTP algorithm.

    @param size The encryption key size.

    @return The encryption key.
*/
unsigned char *randomKey(int size);

/**
    \brief Fucntion to decrypt the plaintext in OTP algorithm.

    @param str The encrypted plaintext.  
    @param key The encryption key.
    @param size User's input text size.

    @return The decrypted message.
*/
unsigned char *OTP_decrypt(unsigned char *str, unsigned char *key, int size);

/**
    \brief Fucntion to encrypt the given text in OTP algorithm.

    @param str The text to be encryption.
    @param key The encryption key.
    @param size User's input text size.

    @return The encrypted message.
*/
unsigned char *OTP_encrypt(char *str, char *key, int size);

// ###############################         CAESAR'S ALGORITHM FUNCTIONS       ###############################

/**
    \brief Fucntion to get the category of a character depending on its ascci code 

    @param ch The character want to find it's category

    @return The category in enum category formation (number, capital, lower)
*/
enum category findCharacterCategory(char ch);

/**
    \brief Fucntion to shift a given character into the given characters

    @param ch  Character to be shifted.
    @param shiftIndicator The positive number indicating the number of shifted positions.
    @param categoryStart The character's category start in ascii code .
    @param categoryEnd The character's category end in ascii code .
    @param nextCategoriesSplitter  The lenth of the first category after the character's category
    @param changeCategoryLimit The total length of the other two categories.
    @param firstGap The gap between the characters category and the next category .
    @param secondGap The gap between the categories after character's category .

    @return The shifted character.
*/
char shiftCharacter(char ch, int shiftIndicator, int categoryStart, int categoryEnd, int nextCategoriesSplitter,
                    int changeCategoryLimit, int firstGap, int secondGap);

/**
    \brief Fucntion to encrypt the given text in caesar's algorithm.

    @param str The text to be encrypted.  
    @param shiftIndicator A positive number indicating the number of shifted positions
    @param size User's input text size.

    @return The encrypted plaintext.
*/
unsigned char *CS_encrypt(char *str, int shiftIndicator, int size);

/**
    \brief Fucntion to decrypt the plaintext in caesar's algorithm.

    @param str The encrypted text.  
    @param shiftIndicator A positive number indicating the number of shifted positions
    @param size User's input text size.

    @return The decrypted message.
*/
unsigned char *CS_decrypt(unsigned char *str, int shiftIndicator, int size);

// ###############################         VIGENERE'S ALGORITHM FUNCTIONS     ###############################

/**
    \brief Function to find the right character depend on the tabula recta.

    @param column The character for indexing the columns of the table.

    @param row The character for indexing on the rows of the table.

    @return The tabula recta point's character found by the two given characters.
    
*/
char shiftViaTabulaRecta(char column, char row);

/**
    \brief Function to locate the column character in tabula recta depending on the row 
    and the character where the selected column meets the selected row. 

    @param depth The character where the selected column meets the selected row. 
    @param row The character for indexing on the rows of the table.

    @return The column character of the tabula recta.
*/
char findColumnTabulaRecta(char depth, char row);

/**
    \brief Fucntion to encrypt the given text in vigenere's  algorithm

    @param str The text to be encrypted.
    @param key The key given from user in uppercase.

    @return The encrypted plaintext

*/
unsigned char *VG_encrypt(char *str, char *key);

/**
    \brief Fucntion to decrypt the plaintext in vigenere's algorithm

    @param str The encrypted plaintext.
    @param key The key given from user in uppercase.

    @return The decrypted message
*/
unsigned char *VG_decrypt(unsigned char *str, char *key);

#endif //SIMPLE_CRYPTO_H
