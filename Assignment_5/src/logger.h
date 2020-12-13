#ifndef _LOGGER_H
#define _LOGGER_H

#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <openssl/md5.h>
#include "utils.h"

#define ONE_LINE_LOGS 1
#define LOG_FILE_PATH "../logs/file_logging.log"

/**
 * @brief Enumerator access type. For file creation, the access type is 0. For file open,
 *  the access type is 1. For file write, the access type is 2.
 */
enum AccessType
{
	creation,
	opening,
	writing
};

/**
 * @brief Is a struct for all the information a log entry should contain
 * 
 * @param UID				   		The unique user ID assigned by the system to a user
 * @param filename   				The path and name of the accessed file. 
 * @param date			   			The date that the action occurred. 
 * @param timestamp			   		The time that the action occurred. 
 * @param AccessType			   	For file creation 0. For file open 1. For file write 2 
 * @param isActionDenied		    Reports if the action was denied to the user with no access privileges 
 * @param fileFingerprint			The digital fingerprint of the file the time the event occurred. 
 *
 */
typedef struct logEntry
{
	int UID;					   /** The unique user ID assigned by the system to a user */
	char filename[3 * BUF_LEN];	   /** The path and name of the accessed file. */
	char date[BUF_LEN];			   /** The date that the action occurred. */
	char timestamp[BUF_LEN];	   /** The time that the action occurred. */
	enum AccessType access;		   /** For file creation 0. For file open 1. For file write 2 */
	int isActionDenied;			   /** Reports if the action was denied to the user with no access privileges */
	char fileFingerprint[BUF_LEN]; /** ​ The digital fingerprint of the file the time the event occurred. */
} logEntry;

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

/**
 * @brief Creates and returns the log message depending on the predefined ONE_LINE_LOGS
 * 
 * @param log The logEntry want to write on file
 * @return char* The logentry in string format 
 */
char * getLogMessage(logEntry log);

#endif /* _LOGGER_H */