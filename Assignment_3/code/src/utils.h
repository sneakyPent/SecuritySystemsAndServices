#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>
#include <stdlib.h>

enum mode{info,error,success};
extern int debug;

/*
 * Prints the hex value of the input, 16 values per line
 *
 * arg0: data
 * arg1: data len
 */
void
print_hex(unsigned char *, size_t);

/**
 * @brief Function for taking the public and private keys seperatety
 * 
 * @param key 
 * @return size_t* 
 */
size_t * splitKey(unsigned char *key);

/*
 * Prints the input as string
 *
 * arg0: data
 * arg1: data len
 */
void
print_string(unsigned char *, size_t);


/*
 * Prints the usage message
 */
void
usage(void);


/*
 * Checks the validity of the arguments
 *
 * arg0: path to input file
 * arg1: path to output file
 * arg2: path to key file
 * arg3: operation mode
 */
void
check_args(char *, char *, char *, int);


/**
 * @brief Function to print messages in debug mode
 * 
 * @param str 
 * @param mode The mode in which we want to print (info, success, error) 
 */
void print(char * str, enum mode );


/**
 * @brief Function to read, write and append files.
 * 
 * @param fileName          The filename want to interact with.
 * @param mode              rb for reading or wb for writting
 * @param messsage         The message we want to write in write mode
 * @param plaintextLength   The length of the plaintext
 */
void *fileManager(char *fileName, char *mode, void *messsage, long *plaintextLength);

#endif /* _UTILS_H */
