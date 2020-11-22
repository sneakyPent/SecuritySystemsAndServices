#include <stdio.h>
#include <string.h>
#include "utils.h"

char filenames[3][BUF_LEN] = {
	"../accessibleFiles/afile_1.txt",
	"../accessibleFiles/afile_2.txt",
	"../accessibleFiles/afile_3.txt",
};

char nonPermFiles[8][BUF_LEN] = {
	"../inaccessibleFiles/nfile_1.txt",
	"../inaccessibleFiles/nfile_2.txt",
	"../inaccessibleFiles/nfile_3.txt",
	"../inaccessibleFiles/nfile_4.txt",
	"../inaccessibleFiles/nfile_5.txt",
	"../inaccessibleFiles/nfile_6.txt",
	"../inaccessibleFiles/nfile_7.txt",
	"../inaccessibleFiles/nfile_8.txt",
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
		if (file != NULL)
		{
			fclose(file);
		}
	}
	for (i = 0; i < nonPerFiles; i++)
	{
		file = fopen(nonPermFiles[i], "r");
		if (file != NULL)
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
		if (file != NULL)
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
		if (file != NULL)
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
			if (file != NULL)
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
		if (file != NULL)
		{
			char msg[BUF_LEN];
			sprintf(msg, " Append my uid %d", getuid());
			bytes = fwrite(msg, strlen(msg), 1, file);
			fclose(file);
		}
	}
}

int main(int argc, char *argv[])
{
	int ch;
	nonPerFiles = 2;
	while ((ch = getopt(argc, argv, "m")) != -1)
	{
		switch (ch)
		{
		case 'm':
			nonPerFiles = 8;
			break;
		default:
			nonPerFiles = 2;
			break;
		}
	}
	files = sizeof(filenames) / sizeof(filenames[0]);
	readFiles();
	writeFiles();
	appendTest();

	argc -= optind;
	argv += optind;
	return 0;
}
