#define _GNU_SOURCE

#include "logger.h"
#include "utils.h"

FILE *
fopen(const char *path, const char *mode)
{

	FILE *original_fopen_ret;
	FILE *(*original_fopen)(const char *, const char *);
	int exists = 1;
	// Get the absolute path of the given file
	char absolutePath[BUF_LEN];
   	realpath(path, absolutePath);
	
	// If file does not exist update log file for file creation
	if (access(absolutePath, F_OK) == -1)
		exists = 0;
	/* call the original fopen function */
	original_fopen = dlsym(RTLD_NEXT, "fopen");
	original_fopen_ret = (*original_fopen)(absolutePath, mode);

	if (original_fopen_ret)
		if (exists == 0)	// Update log file for creating file
			logFileUpdate(initLogs(absolutePath, creation, original_fopen_ret, mode));
		else				// Update log file for opening file
			logFileUpdate(initLogs(absolutePath, opening, original_fopen_ret, mode));
	else
		logFileUpdate(initLogs(absolutePath, opening, original_fopen_ret, mode));

	return original_fopen_ret;
}

size_t
fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	// Get the filename from File pointer
	char *filename = getFilesName(stream);

	size_t original_fwrite_ret;
	size_t (*original_fwrite)(const void *, size_t, size_t, FILE *);

	/* call the original fwrite function */
	original_fwrite = dlsym(RTLD_NEXT, "fwrite");
	original_fwrite_ret = (*original_fwrite)(ptr, size, nmemb, stream);
	// Update log file for writing file
	logFileUpdate(initLogs(filename, writing, stream, "w"));

	return original_fwrite_ret;
}

logEntry initLogs(const char *path, enum AccessType aType, FILE *file, const char *mode)
{

	FILE *(*original_fopen)(const char *, const char *);
	original_fopen = dlsym(RTLD_NEXT,"fopen");
	char **dateAndTime = getCurrentDateAndTime();
	logEntry le;

	le.UID = getuid();
	strcpy(le.filename, path);
	strcpy(le.timestamp, dateAndTime[0]);
	strcpy(le.date, dateAndTime[1]);
	le.access = aType;
	le.isActionDenied = getAccess(path, mode);
	
	// get MD5 fingerprint
	if (getAccess(path, "r") == 0){
		char *filename;
		filename = getFilesName(file);
		fflush(file);

		FILE *fwrite_text = original_fopen(filename,"r");
		
		if (fwrite_text)
		{	
			long lSize;
			fseek(fwrite_text, 0L, SEEK_END);
			lSize = ftell(fwrite_text);
			rewind(fwrite_text);
			unsigned char *retext = NULL;
			retext = malloc(sizeof(char) * lSize);

			long readlength = fread(retext, sizeof(char), lSize , fwrite_text);
			unsigned char fingerprint[MD5_DIGEST_LENGTH];
			MD5(retext, readlength, fingerprint);
			strcpy(le.fileFingerprint, stringToHex(fingerprint, sizeof(fingerprint)));
		}
		else
			strcpy(le.fileFingerprint, "00000000000000000000000000000000");
		fclose(fwrite_text);
	}
	else
		strcpy(le.fileFingerprint, "00000000000000000000000000000000");
		
	return le;
}

int logFileUpdate(logEntry log)
{
	// FILE *original_fopen_ret;
	FILE *(*original_fopen)(const char *, const char *);
	size_t (*original_fwrite)(const void *, size_t, size_t, FILE *);

	/* call the original fopen function */
	original_fopen = dlsym(RTLD_NEXT, "fopen");
	/* call the original fwrite function */
	original_fwrite = dlsym(RTLD_NEXT, "fwrite");

	char logMessage[4096];

	sprintf(logMessage,
			"\n"
			"UID· %d\n"
			"File name· %s\n"
			"Date· %s\n"
			"Timestamp· %s\n"
			"Access t​ype· %d \n"
			"Action denied· %d\n"
			"File fingerprint· %s \n"
			"\n------------------------------------------------------------------------\n",
			log.UID, log.filename, log.date, log.timestamp, log.access, log.isActionDenied, log.fileFingerprint);

	FILE *logFile = original_fopen(LOG_FILE_PATH, "a");
	if (logFile == NULL)
	{
		char msg[50] = "Cannot find file ";
		strcat(msg, LOG_FILE_PATH);
		print(msg, error);
	}

	long writelength = original_fwrite(logMessage, sizeof(char), strlen(logMessage), logFile);
	if (writelength != strlen(logMessage))
		print("Writing file error", error);

	fclose(logFile);
	return 1;
}


FILE *
fopen64(const char *path, const char *mode)
{

	// return fopen(path, mode);
	FILE *original_fopen_ret;
	FILE *(*original_fopen)(const char *, const char *);
	int exists = 1;
	// Get the absolute path of the given file
	char absolutePath[BUF_LEN];
   	realpath(path, absolutePath);
	
	// If file does not exist update log file for file creation
	if (access(absolutePath, F_OK) == -1)
		exists = 0;
	/* call the original fopen function */
	original_fopen = dlsym(RTLD_NEXT, "fopen64");
	original_fopen_ret = (*original_fopen)(absolutePath, mode);

	if (original_fopen_ret)
		if (exists == 0)	// Update log file for creating file
			logFileUpdate(initLogs(absolutePath, creation, original_fopen_ret, mode));
		else				// Update log file for opening file
			logFileUpdate(initLogs(absolutePath, opening, original_fopen_ret, mode));
	else
		logFileUpdate(initLogs(absolutePath, opening, original_fopen_ret, mode));

	return original_fopen_ret;
}