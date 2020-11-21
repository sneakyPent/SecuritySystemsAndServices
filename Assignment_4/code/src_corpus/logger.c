#define _GNU_SOURCE

#include "logger.h"
#include "utils.h"

FILE *
fopen(const char *path, const char *mode)
{

	FILE *original_fopen_ret;
	FILE *(*original_fopen)(const char *, const char *);
	int exists = 1;

	// If file does not exist update log file for file creation
	if (access(path, F_OK) == -1)
	{
		print("File does not exist", info);
		exists = 0;
	}
	/* call the original fopen function */
	original_fopen = dlsym(RTLD_NEXT, "fopen");
	original_fopen_ret = (*original_fopen)(path, mode);

	if (original_fopen_ret)
	{
		if (exists == 0)
		{
			print("File creation", info);
			logFileUpdate(initLogs(path, creation, original_fopen_ret, mode));
		}
		else
		{
			// Update log file for opening file
			print("File opening", info);
			logFileUpdate(initLogs(path, opening, original_fopen_ret, mode));
		}
	}
	else
		logFileUpdate(initLogs(path, opening, original_fopen_ret, mode));

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

char **getCurrentDateAndTime()
{
	time_t current = time(NULL); /* Gets GMT time as a time_t. */
	if (current == -1)
		print("The time() function failed", error);
	struct tm *pLocal = localtime(&current); /* Converts to local time in broken down format. */
	if (pLocal == NULL)
		print("The localtime() function failed", error);
	char **dateAndTime;

	dateAndTime = malloc(sizeof(char *) * 2);

	for (int i = 0; i < 2; i++)
		dateAndTime[i] = malloc(BUF_LEN);

	strftime(dateAndTime[0], BUF_LEN, "%T", pLocal);
	strftime(dateAndTime[1], BUF_LEN, "%d/%m/%Y", pLocal);
	return dateAndTime;
}

logEntry initLogs(const char *path, enum AccessType aType, FILE *file, const char *mode)
{

	char **dateAndTime = getCurrentDateAndTime();
	logEntry le;

	le.UID = getuid();
	strcpy(le.filename, path);
	strcpy(le.timestamp, dateAndTime[0]);
	strcpy(le.date, dateAndTime[1]);
	le.access = aType;
	le.isActionDenied = getAccess(path, mode);

	//get MD5 fingerprint
	if (file)
	{
		long lSize;
		fseek(file, 0L, SEEK_SET);
		fseek(file, 0L, SEEK_END);
		lSize = ftell(file);
		fseek(file, 0L, SEEK_SET);
		unsigned char *retext = NULL;
		retext = malloc(sizeof(char) * lSize);
		long readlength = fread(retext, sizeof(char), lSize, file);
		if (readlength == lSize)
		{
			unsigned char fingerprint[MD5_DIGEST_LENGTH];
			MD5(retext, readlength, fingerprint);
			strcpy(le.fileFingerprint, stringToHex(fingerprint, sizeof(fingerprint)));
		}
		else
			strcpy(le.fileFingerprint, "00000000000000000000000000000000");
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
			"UID: %d\n"
			"File name: %s\n"
			"Date: %s\n"
			"Timestamp: %s\n"
			"Access t​ype: %d \n"
			"Action denied: %d\n"
			"File fingerprint: %s \n"
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