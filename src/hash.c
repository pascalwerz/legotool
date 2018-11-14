/* this file is part of legotool, by xvi */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>


#define K	0x01000193
#define Kp	0x359c449b	// modular inverse of K

#define COMPLEMENT 0


// Fowler-Noll-Vo FNV1 hash

void usage(char *me, int exitCode)
{
fprintf(stderr, "usage: [options] %s text\n", me);
fprintf(stderr, "       -i            keep case, do not force uppercase\n");
fprintf(stderr, "       -k constant   change multiplication constant\n");
fprintf(stderr, "       -s seed       change base hash value\n");
fprintf(stderr, "       -x            raw hex output\n");
fprintf(stderr, "       -r hash       reverse mode ('remove' trailing text from hash)\n");

if (exitCode) exit(exitCode);
}



int main(int argc, char **argv)
{
uintmax_t hash;
uintmax_t seed = 0x811c9dc5;
uintmax_t multConst = 0x1000193;
uintmax_t multConstReverse = 0x359c449b;
int rawDisplay = 0;
int keepCase = 0;
int reverse = 0;
char *me;
int ch;


me = argv[0];
while ((ch = getopt(argc, argv, "ik:r:s:x")) != -1)
	{
	switch (ch)
		{
	case 'k':
		multConst = strtoumax(optarg, NULL, 16);
		break;
	case 's':
		seed = strtoumax(optarg, NULL, 16);
		break;
	case 'i':
		keepCase = !keepCase;
		break;
	case 'r':
		reverse = 1;
		seed = strtoumax(optarg, NULL, 16);
		break;
	case 'x':
		rawDisplay = !rawDisplay;
		break;
	default:
		usage(me, 1);
		break;
		}
	}
argc -= optind;
argv += optind;

if (reverse)
	{
	if (argc < 1) return 1;

//printf("argc %u argv[0] %s seed 0x%08jx\n", argc, argv[0], seed);

	for (int j = 0; j < argc; j++)
		{
		hash = seed;
		for (int i = strlen(argv[j]) - 1; i >= 0; i--)
			{
			if (keepCase)
				hash ^= argv[j][i];
			else
				hash ^= toupper(argv[j][i]);
			hash = hash * multConstReverse;
			hash &= 0xffffffff;
			}
		if (rawDisplay)
			printf("0x%08jx\n", hash);
		else
			{
			printf("0x%08jx is 0x%08jx without trailing \"%s\"\n", hash, seed, argv[j]);
			}
		}
	}
else
	{
	if (argc < 1) return 1;

	for (int j = 0; j < argc; j++)
		{
		hash = seed;
		for (int i = 0; i < strlen(argv[j]); i++)
			{
			hash = hash * multConst;
			if (keepCase)
				hash ^= argv[j][i];
			else
				hash ^= toupper(argv[j][i]);
			hash &= 0xffffffff;
			}

		if (rawDisplay)
			printf("%08jx\n", hash);
		else
			{
			printf("{ 0x%08jx, gameAny, -1, 0x0, \"", hash);
			for (int i = 0; i < strlen(argv[j]); i++) if (argv[j][i] == '\\') printf("\\\\"); else if (argv[j][i] == '"') printf("\\\""); else printf("%c", argv[j][i]);
			printf("\", \"\", NULL },\n");
			}
		}
	}
}