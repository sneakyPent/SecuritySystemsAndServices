#include <getopt.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>

void usage(void)
{
	printf(
		"\n"
		"usage:\n"
		"\t./test \n"
		"\nOptions:\n\n"
		"-p <payload>,\t\t The payload in hex form\n"
		"-h, \t\t\t Help message\n\n");

	exit(1);
}

int main(int argc, char **argv)
{
	unsigned char payload[40];
	int ch;
	if (argc < 2 || argc > 3)
		usage();
	while ((ch = getopt(argc, argv, "p:")) != -1)
	{
		switch (ch)
		{
		case 'p':
			strcpy(payload,optarg);
			break;
		case 'h':
			break;
		default:
			usage();
		}
	}

	argc -= optind;
	argv += optind;
	
	int (*foo)() = (int (*)())payload;
	foo();
	return 0;

}