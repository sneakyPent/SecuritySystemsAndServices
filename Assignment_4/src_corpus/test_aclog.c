#include <stdio.h>
#include <string.h>
#include "utils.h"




void readFiles(){
	int i;
	size_t bytes;
	FILE *file;
	int files = 8;
	char filenames[8][BUF_LEN] = {
		"../testFiles/FILE_1.txt",
		"../testFiles/FILE_2.txt",
		"../testFiles/FILE_3.txt",
		"../testFiles/FILE_4.txt",
		"../testFiles/FILE_5.txt",
		"../testFiles/FILE_6.txt",
		"../testFiles/FILE_7.txt",
		"../testFiles/FILE_8.txt",
	};
	char nonPermFiles[8][BUF_LEN] = {
		"../testFiles/N_FILE_1.txt",
		"../testFiles/N_FILE_2.txt",
		"../testFiles/N_FILE_3.txt",
		"../testFiles/N_FILE_4.txt",
		"../testFiles/N_FILE_5.txt",
		"../testFiles/N_FILE_6.txt",
		"../testFiles/N_FILE_7.txt",
		"../testFiles/N_FILE_8.txt",
	};

	for (i = 0; i < files; i++) {
		file = fopen(filenames[i], "r");
		print("Read", info);
		if (file == NULL) 
			printf("fopen error\n");
		else {
			fclose(file);
		}

	}
	for (i = 0; i < files; i++) {
		file = fopen(nonPermFiles[i], "r");
		print("Read", info);
		if (file == NULL) 
			printf("fopen error\n");
		else {
			fclose(file);
		}

	}
}

void writeFiles(){
	
	int i;
	size_t bytes;
	FILE *file;
	int files = 8;
	char filenames[8][BUF_LEN] = {
		"../testFiles/FILE_1.txt",
		"../testFiles/FILE_2.txt",
		"../testFiles/FILE_3.txt",
		"../testFiles/FILE_4.txt",
		"../testFiles/FILE_5.txt",
		"../testFiles/FILE_6.txt",
		"../testFiles/FILE_7.txt",
		"/lnx/sneaky/testFiles/FILE_8.txt",
	};
	char nonPermFiles[8][BUF_LEN] = {
		"../testFiles/N_FILE_1.txt",
		"../testFiles/N_FILE_2.txt",
		"../testFiles/N_FILE_3.txt",
		"../testFiles/N_FILE_4.txt",
		"../testFiles/N_FILE_5.txt",
		"../testFiles/N_FILE_6.txt",
		"../testFiles/N_FILE_7.txt",
		"../testFiles/N_FILE_8.txt",
	};

	for (i = 0; i < files; i++) {
		file = fopen(filenames[i], "w+");
		print("Write", info);
		if (file == NULL) 
			printf("fopen error\n");
		else {
			char msg[BUF_LEN];
			sprintf(msg,"%d", getuid());
			bytes = fwrite(msg, strlen(msg), 1, file);
			fclose(file);
		}

	}
	for (i = 0; i < files; i++) {
		file = fopen(nonPermFiles[i], "w+");
		print("Write", info);
		if (file == NULL) 
			printf("fopen error\n");
		else {
			char msg[BUF_LEN];
			sprintf(msg,"%d", getuid());
			bytes = fwrite(msg, strlen(msg), 1, file);
			fclose(file);
		}

	}

}

	/* add your code here */
	/* ... */
	/* ... */
	/* ... */
	/* ... */


}
