#include "utils.h"

int debug = 0;

/*
 * Prints the hex value of the input
 *
 * arg0: data
 * arg1: data len
 */
void
print_hex(unsigned char *data, size_t len)
{
	size_t i;

	if (!data)
		printf("NULL data\n");
	else {
		for (i = 0; i < len; i++) {
			if (!(i % 16) && (i != 0))
				printf("%02X ", data[i]);
			printf("%02X ", data[i]);
		}
		printf("\n");
	}
}

size_t * splitKey(unsigned char *key)
{

	int partlen = sizeof(size_t);
	char n_str[sizeof(size_t)] = "";
	char d_e_str[sizeof(size_t)] = "";
	char str1[80], str2[80];
	size_t *nde = malloc(2*sizeof(size_t));
	for (size_t i = 2 * partlen; i > 8; i--)
	{
		sprintf(str2, "%02X", key[i - 1]);
		strcat(d_e_str, str2);
	}
	nde[0] = strtoul(d_e_str, NULL, 16);
	for (size_t j = partlen; j > 0; j--)
	{
		sprintf(str1, "%02X", key[j - 1]);
		strcat(n_str, str1);
	}
	nde[1] = strtoul(n_str, NULL, 16);
	return nde;
}
/*
 * Prints the input as string
 *
 * arg0: data
 * arg1: data len
 */
void
print_string(unsigned char *data, size_t len)
{
	size_t i;

	if (!data)
		printf("NULL data\n");
	else {
		for (i = 0; i < len; i++)
			printf("%c", data[i]);
		printf("\n");
	}
}


/*
 * Prints the usage message
 */
void
usage(void)
{
	printf(
	    "\n"
	    "Usage:\n"
	    "    assign_3 -g \n" 
	    "    assign_3 -i in_file -o out_file -k key_file [-d | -e]\n" 
	    "    assign_3 -h\n"
	);
	printf(
	    "\n"
	    "Options:\n"
	    " -i    path    Path to input file\n"
	    " -o    path    Path to output file\n"
	    " -k    path    Path to key file\n"
	    " -d            Decrypt input and store results to output\n"
	    " -e            Encrypt input and store results to output\n"
	    " -g            Generates a keypair and saves to 2 files\n"
	    " -h            This help message\n"
	);
	exit(EXIT_FAILURE);
}


/*
 * Checks the validity of the arguments
 *
 * arg0: path to input file
 * arg1: path to output file
 * arg2: path to key file
 * arg3: operation mode
 */
void
check_args(char *input_file, char *output_file, char *key_file, int op_mode)
{
	if ((!input_file) && (op_mode != 2)) {
		printf("Error: No input file!\n");
		usage();
	}

	if ((!output_file) && (op_mode != 2)) {
		printf("Error: No output file!\n");
		usage();
	}

	if ((!key_file) && (op_mode != 2)) {
		printf("Error: No user key!\n");
		usage();
	}

	if (op_mode == -1) {
		printf("Error: No mode\n");
		usage();
	}
}


void print(char *str, enum mode md)
{
    switch (md)
    {
    case error:
        printf("\033[1;31m");
        printf("[ERROR]: \033[0m %s\n", str);
        exit(EXIT_FAILURE);
        break;
    case info:
        if (debug == 1)
        {
            printf("\033[0;36m");
            printf("[INFO]: \033[0m %s\n", str);
        }
        break;
    case success:
        printf("\033[0;32m");
        printf("%s\033[0m\n", str);
        break;
    default:
        printf("%s\n", str);
        break;
    }
}

void *fileManager(char *fileName, char *mode, void *messsage, long *plaintextLength)
{

	FILE *fp = NULL;
	void *retext = NULL;
	// print("Opening file.", info);
	fp = fopen(fileName, mode);
	if (fp == NULL)
	{
		char msg[50] = "Cannot find file ";
		strcat(msg, fileName);
		print(msg, error);
	}
	if (strcmp(mode, "r") == 0)
	{
		long flen;

		/*Calculate the size of the plaintext*/
		fseek(fp, 0L, SEEK_END);
		flen = ftell(fp);
		fseek(fp, 0L, SEEK_SET);
		/* allocate memory for plaintext storing*/
		retext = malloc(sizeof(char) * flen);
		/*read plaintext*/
		// print("Reading file...", info);
		long readlength = fread(retext, sizeof(char), flen, fp);
		// if (readlength != flen)
		//     print("Reading error", error);
		*plaintextLength = flen;
		// print("Read file.", info);
	}
	else if (strcmp(mode, "w") == 0)
	{
		long writelength = fwrite(messsage, sizeof(char), *plaintextLength, fp);
		if (writelength != *plaintextLength)
			print("writing error", error);
		// print("write file.", info);
	}
	else if (strcmp(mode, "a") == 0)
	{
		long writelength = fwrite(messsage, sizeof(char), *plaintextLength, fp);
		if (writelength != *plaintextLength)
			print("writing error", error);
		// print("append file.", info);
	}

	else
		print("Give valid File manager mode (write->w,  read->r,  append->a)", error);
	// print("Closing File...", info);
	fclose(fp);
	// print("File closed.", info);
	return retext;
}
