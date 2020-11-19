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
#include "utils.h"

#define LOG_FILE_PATH "../logs/file_logging.log"

/**
 * @brief Enumerator access type. For file creation, the access type is 0. For file open,
 *  the access type is 1. For file write, the access type is 2.
 * 
 */
enum AccessType
{
	creation, opening, writing
};

/**
 * @brief Is a struct for all the information a log entry should contain
 *   
 * 
 */
typedef struct logEntry
{
	int UID;					   /** The unique user ID assigned by the system to a user */
	char filename[3*BUF_LEN];		   /** The path and name of the accessed file. */
	char date[BUF_LEN];			   /** The date that the action occurred. */
	char timestamp[BUF_LEN];	   /** The time that the action occurred. */
	enum AccessType access;		   /** For file creation 0. For file open 1. For file write 2 */
	int isActionDenied;			   /** Reports if the action was denied to the user with no access privileges */
	char fileFingerprint[BUF_LEN]; /** ​ The digital fingerprint of the file the time the event occurred. */
}logEntry;

/**
* @brief Get the Current Date And Time
 * 
 * @return char** 2D array [2][BUF_LEN] char[0]=time char[1]=date
 */
char **getCurrentDateAndTime();


/**
 * @brief Init the entry logs for current event.
 * 
 * @param path Filename
 * @param mode The original fopen mode.
 * @return struct logEntry 
 */
logEntry initLogs(const char *path, enum AccessType aType, FILE *file, const char *mode);


/**
 * @brief Updates the LOG_FILE_PATH defined in logger.h with the given log entry information
 * 
 * @return int 1 for success 0 for error
 */
int logFileUpdate(logEntry);
#endif /* _LOGGER_H */