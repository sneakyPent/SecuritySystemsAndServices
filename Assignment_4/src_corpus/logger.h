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

/**
 * @brief Is a struct for all the information a log entry should contain
 *   
 * 
 */
struct logEntry
{
	int UID; /** The unique user ID assigned by the system to a user */
	char filename[4096]; /** The path and name of the accessed file. */
	char date[BUF_LEN]; /** The date that the action occurred. */
	char timestamp[BUF_LEN]; /** The time that the action occurred. */
	enum AccessType access; /** For file creation 0. For file open 1. For file write 2 */
	int isActionDenied; /** Reports if the action was denied to the user with no access privileges */
	unsigned char fileFingerprint;  /** ​ The digital fingerprint of the file the time the event occurred. */

};




#endif /* _LOGGER_H */