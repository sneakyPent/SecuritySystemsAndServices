#include <stdio.h>
#include <string.h>
#include "utils.h"


char filenames[3][BUF_LEN] = {
	"FILE_1.txt",
	"FILE_2.txt",
	"FILE_3.txt",
};
char nonPermFiles[8][BUF_LEN] = {
	"FILE_N_1.txt",
	"FILE_N_2.txt",
	"FILE_N_3.txt",
	"FILE_N_4.txt",
	"FILE_N_5.txt",
	"FILE_N_6.txt",
	"FILE_N_7.txt",
	"FILE_N_8.txt",
};

static int nonPerFiles, files;

void readFiles()
{
	int i;
	size_t bytes;
	FILE *file;
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
	for (i = 0; i < nonPerFiles; i++)
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
	for (i = 0; i < nonPerFiles; i++)
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
	for (i = 0; i < nonPerFiles; i++)
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

	nonPerFiles = sizeof(nonPermFiles) / sizeof(nonPermFiles[0]);
	files = sizeof(filenames) / sizeof(filenames[0]);
	readFiles();
	writeFiles();
	appendTest();
	// readFiles();
}
