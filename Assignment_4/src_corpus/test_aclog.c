#include <stdio.h>
#include <string.h>
#include "utils.h"

char filenames[8][BUF_LEN] = {
	"FILE_1.txt",
	"FILE_2.txt",
	"FILE_3.txt",
	"FILE_4.txt",
	"FILE_5.txt",
	"FILE_6.txt",
	"FILE_7.txt",
	"FILE_8.txt",
};
char nonPermFiles[8][BUF_LEN] = {
	"N_FILE_1.txt",
	"N_FILE_2.txt",
	"N_FILE_3.txt",
	"N_FILE_4.txt",
	"N_FILE_5.txt",
	"N_FILE_6.txt",
	"N_FILE_7.txt",
	"N_FILE_8.txt",
};

void readFiles()
{
	int i;
	size_t bytes;
	FILE *file;
	int files = 8;
	for (i = 0; i < files; i++)
	{
		file = fopen(filenames[i], "r");
		print("Read", info);
		if (file == NULL)
			printf("fopen error\n");
		else
		{
			fclose(file);
		}
	}
	for (i = 0; i < files; i++)
	{
		file = fopen(nonPermFiles[i], "r");
		print("Read", info);
		if (file == NULL)
			printf("fopen error\n");
		else
		{
			fclose(file);
		}
	}
}

void writeFiles()
{

	int i;
	size_t bytes;
	FILE *file;
	int files = 8;
	for (i = 0; i < files; i++)
	{
		file = fopen(filenames[i], "w+");
		print("Write", info);
		if (file == NULL)
			printf("fopen error\n");
		else
		{
			char msg[BUF_LEN];
			sprintf(msg, "%d", getuid());
			bytes = fwrite(msg, strlen(msg), 1, file);
			fclose(file);
		}
	}
	for (i = 0; i < files; i++)
	{
		file = fopen(nonPermFiles[i], "w+");
		print("Write", info);
		if (file == NULL)
			printf("fopen error\n");
		else
		{
			char msg[BUF_LEN];
			sprintf(msg, "%d", getuid());
			bytes = fwrite(msg, strlen(msg), 1, file);
			fclose(file);
		}
	}
}
void appendTest()
{
	int i;
	size_t bytes;
	FILE *file;
	int files = 8;
	for (i = 0; i < files; i++)
	{	
		// append only in odd files
		if (i % 2 == 0)
		{
			file = fopen(filenames[i], "a+");
			print("Write", info);
			if (file == NULL)
				printf("fopen error\n");
			else
			{
				char msg[BUF_LEN];
				sprintf(msg, " Append my uid %d", getuid());
				bytes = fwrite(msg, strlen(msg), 1, file);
				fclose(file);
			}
		}
	}
	for (i = 0; i < files; i++)
	{
		file = fopen(nonPermFiles[i], "a+");
		print("Write", info);
		if (file == NULL)
			printf("fopen error\n");
		else
		{
			char msg[BUF_LEN];
			sprintf(msg, " Append my uid %d", getuid());
			bytes = fwrite(msg, strlen(msg), 1, file);
			fclose(file);
		}
	}
}

int main()
{
	readFiles();
	writeFiles();
	appendTest();
	readFiles();
}
