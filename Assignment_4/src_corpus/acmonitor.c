#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "logger.h"
#include "utils.h"



int isHashChanged(char *prevHash, char *newHash)
{
	return strcmp(prevHash, newHash) == 0 ? 0 : 1;
}

int isGivenFile(char *givenFile, char *currentFile)
{
	int rvalue = -1; 
	int curFileLen = strlen(currentFile);
	int givenFileLen = strlen(givenFile);
	if(curFileLen > givenFileLen)
		rvalue = strcmp(givenFile, currentFile + curFileLen - givenFileLen) == 0 ? 1 : 0;
	else
		rvalue = strcmp(currentFile, givenFile) == 0 ? 1 : 0;;
	return rvalue;
}

logEntry getNextLogEntry(FILE *log)
{

	char line[BUF_LEN];
	logEntry tmpLog = {-1, "", "", "", -1, -1};
	while (fgets(line, sizeof(line), log))
	{
		if (strstr(line, "UID") != NULL)
			tmpLog.UID = atoi(getLineInfo(line));
		if (strstr(line, "File name") != NULL)
			strcpy(tmpLog.filename, getLineInfo(line));
		if (strstr(line, "Date") != NULL)
			strcpy(tmpLog.date, getLineInfo(line));
		if (strstr(line, "Timestamp") != NULL)
			strcpy(tmpLog.timestamp, getLineInfo(line));
		if (strstr(line, "Access t​ype") != NULL)
			tmpLog.access = atoi(getLineInfo(line));
		if (strstr(line, "Action denied") != NULL)
			tmpLog.isActionDenied = atoi(getLineInfo(line));
		if (strstr(line, "File fingerprint") != NULL)
			strcpy(tmpLog.fileFingerprint, getLineInfo(line));
		if (strstr(line, "----------------------------") != NULL)
			return tmpLog;
	}
	return tmpLog;
}

void usage(void)
{
	printf(
		"\n"
		"usage:\n"
		"\t./acmonitor \n"
		"Options:\n"
		"-m, Prints malicious users\n"
		"-i <filename>, Prints table of users that modified "
		"the file <filename> and the number of modifications\n"
		"-h, Help message\n\n");

	exit(1);
}

void list_unauthorized_accesses(FILE *log)
{
	logEntry logt;
	userList *users = NULL;
	int modifications = 0;
	while (1)
	{
		logt = getNextLogEntry(log);
		if (logt.UID == -1)
			break;
		if (logt.isActionDenied == 1 && logt.access == opening)
		{
			users = addUser(users, logt.UID, 0, 1,logt.filename);
		}
	};
	printUsers(users, nonPermissions);
}

void list_file_modifications(FILE *log, char *file_to_scan)
{
	logEntry logt;
	userList *users = NULL;
	int modifications = 0;
	char prevHash[BUF_LEN] = "";
	while (1)
	{
		logt = getNextLogEntry(log);
		if (logt.UID == -1)
			break;
		if (isGivenFile(file_to_scan, logt.filename) && logt.isActionDenied == 0)
		{
			users = addUser(
				users,
				logt.UID,
				(isHashChanged(prevHash, logt.fileFingerprint) && (strlen(prevHash) != 0) && logt.access == writing),
				0,
				""
			);
			strcpy(prevHash, logt.fileFingerprint);
		}
	};
	printUsers(users, modifies);
}

int main(int argc, char *argv[])
{

	int ch;
	FILE *log;

	if (argc < 2)
		usage();

	log = fopen(LOG_FILE_PATH, "r");
	if (log == NULL)
	{
		printf("Error opening log file \"%s\"\n", "./log");
		return 1;
	}
	while ((ch = getopt(argc, argv, "hi:m")) != -1)
	{
		switch (ch)
		{
		case 'i':
			list_file_modifications(log, optarg);
			break;
		case 'm':
			list_unauthorized_accesses(log);
			break;
		default:
			usage();
		}
	}

	/* add your code here */
	/* ... */
	/* ... */
	/* ... */
	/* ... */

	fclose(log);
	argc -= optind;
	argv += optind;

	return 0;
}
