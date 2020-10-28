//
// Created by zn on 10/26/20.
//

#ifndef SECURITY_SYSTEMS_ASSIGN_1_H
#define SECURITY_SYSTEMS_ASSIGN_1_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/conf.h>
#include <openssl/cmac.h>

#define BLOCK_SIZE 16
enum mode{info,error,success};

static int debug=0;

/* function prototypes */

/**
 * @brief Prints the hex value of the input, 16 values per line.
 *
 *  @param data The data to be printed.
 *  @param len  The length of the given data
 * 
*/
void print_hex(unsigned char *data, size_t len);

/**
 *  @brief Prints the input as string
 *
 *  @param data The data to be printed.
 *  @param len  The length of the given data
 *
*/
void print_string(unsigned char *data, size_t len);

/**
 * Prints the usage message
 * Describe the usage of the new arguments you introduce
 */
void usage(void);

/**
 *  Checks the validity of the arguments
 *  Check the new arguments you introduce
 */
void check_args(char *, char *, unsigned char *, int, int);

/**
 *  @brief Generates a key using a password
 *
 *  @param password     The password give by user
 *  @param key          The key will be derived by keygen
 *  @param iv
 *  @param bit_mode     The desired key size (128 or 256 bits)
 *
 */
void keygen(unsigned char *password, unsigned char *key, int bit_mode);

/**
 *  @brief Encrypts the data
 *
 *  @param plaintext        The input text to be encrypted
 *  @param plaintext_len    The input text length
 *  @param key              The derived key will be used for encryption
 *  @param ciphertext       The encrypted text will be returned to user
 *  @param bit_mode         The desired key size (128 or 256 bits)
 *  
 */
void encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
             unsigned char *ciphertext, int bit_mode);

/**
 * @brief Decrypts the data and returns the plaintext size
 *
 *  @param ciphertext       The cipher text to be decrypted
 *  @param ciphertext_len   The cipher text length
 *  @param key              The derived key will be used for decryption
 *  @param iv
 *  @param plaintext        The decrypted text will be returned to user
 *  @param bit_mode         The desired key size (128 or 256 bits)
 *
 */
int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *plaintext, int bit_mode);

/**
 * @brief Generates a CMAC
 *
 *  @param data             The given data to be signed
 *  @param data_len         The given data length
 *  @param key              The derived key will be used for signing
 *  @param cmac             The data signing returned to user
 *  @param bit_mode         The desired key size (128 or 256 bits)
 *
 */
void gen_cmac(unsigned char *data, size_t data_len, unsigned char *key,
              unsigned char *cmac, int bit_mode);


int verify_cmac(unsigned char *, unsigned char *);

/* TODO Declare your function prototypes here... */

/**
 *  @brief Get the the desired EVP_Cipher will be used for the key generator depend on bit mode.
 *
 *  /EVP_API_Functions
 *      EVP_get_cipherbyname: return either an EVP_CIPHER structure or NULL if an error occurs
 *
 *  \hash_function_names
 *      -   BIT MODE 128:  hash_function_name="aes-128-ecb";
 *      -   BIT MODE 256:  hash_function_name="aes-256-ecb";
 *
 *
 * @return The evp cipher in given bit mode
 */
const EVP_CIPHER * getAesCipher(int bit_mode);

/**
 * @brief Initialize the context will be used in EVP_EncryptUpdate or EVP_DecryptUpdate
 * 
 * \EVP_API_Functions:
 *  -EVP_EncryptInit_ex() sets up cipher context ctx for encryption                                           
 *  -EVP_CIPHER_CTX_new() create and initialise the context
 * 
 * \hash_function_names
 *      -   BIT MODE 128:  hash_function_name="EVP_aes_128_ebc()";
 *      -   BIT MODE 256:  hash_function_name="EVP_aes_256_ebc()";
 * 
 */
EVP_CIPHER_CTX *contextInit(unsigned char *key, int bit_mode, char mode);

/**
 * @brief Function to read and write files.
 * 
 * @param fileName          The filename want to interact with.
 * @param mode              rb for reading or wb for writting
 * @param plaintext         The plaintext read from the file in read mode or the plaintext want to write in write mode
 * @param plaintextLength   The length of the plaintext
 */
void fileManager(char *fileName, char *mode, unsigned char *plaintext, long *plaintextLength);

void print(char * str, enum mode );

#endif //SECURITY_SYSTEMS_ASSIGN_1_H
 
