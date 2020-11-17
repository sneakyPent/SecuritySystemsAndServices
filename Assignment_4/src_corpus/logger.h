#ifndef _LOGGER_H
#define _LOGGER_H


#include <time.h>
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <openssl/md5.h>

#define BUF_LEN 256
/**
 * @brief Enumerator access type. For file creation, the access type is 0. For file open,
 *  the access type is 1. For file write, the access type is 2.
 * 
 */
enum AccessType {creation = 0, opening = 1, writing = 2};





#endif /* _LOGGER_H */