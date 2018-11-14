/* this file is part of legotool, by xvi */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>



#define K					0x01000193
#define Kp					0x359c449b		// modular inverse of K
#define DEFAULT_HASH_SEED	0x811c9dc5

// some globals
char *alphabet;
char next[256];


int minLength = 1;
int maxLength = 10;



void usage(char *me, int exitCode)
{
fprintf(stderr, "usage : %s [options] hash\n", me);
fprintf(stderr, "       brute force Lego FNV1 hash reversal\n");
fprintf(stderr, "       options:\n");
fprintf(stderr, "       -s length          minimum length\n");
fprintf(stderr, "       -e length          maximum length\n");
fprintf(stderr, "       -a \"characters\"    set alphabet for generated labels\n");
fprintf(stderr, "                          default is \"ABCDEFGHIJKLMNOPQRSTUVWXYZ\"\n");
if (exitCode) exit(exitCode);
}



int try(uint32_t seed, int length)
{
static char label[256];
uint32_t hash[257];
int result;
int where;

result = 0;
for (int i = 0; i < length; i++) label[i] = alphabet[0];
label[length] = 0;

 // initialize hash array
hash[length] = seed;
for (int i = length - 1; i >= 0; i--) hash[i] = (hash[i + 1] ^ label[i]) * Kp;
//printf("a %d ", length); for (int i = 0; i <= length; i++) printf("%08x ", hash[i]); printf("\n");

// printf("1 trying %s, 0x%08jx\n", label, (uintmax_t) hash[0]);
if (hash[0] == 0x811c9dc5)
	{
	result++;
	printf("%3lu { 0x%08x, gameAny, -1, 0x0, \"%s\", \"\", NULL },\n", strlen(label), seed, label);
	}

for ( ; ; )
	{
	// compute next label
	for (where = 0; where < length; where++)
		{
		label[where] = next[(unsigned char) label[where]];
		for (int i = where; i >= 0; i--) hash[i] = (hash[i + 1] ^ label[i]) * Kp;
		if (label[where] != alphabet[0])
			{ // ok, we found next label
			if (hash[0] == 0x811c9dc5)
				{
				result++;
				printf("%3lu { 0x%08x, gameAny, -1, 0x0, \"%s\", \"\", NULL },\n", strlen(label), seed, label);
				}
			break;
			}
		// cascade on preceding character
		}
	if (where >= length) break;	// done for given length
	}

return result;
}



int main(int argc, char **argv)
{
uint32_t seed;
int found;
int ch;
char *me = argv[0];


if (argc < 2) return 1;

alphabet = malloc(26 + 1);
if (!alphabet) { perror(NULL); exit(1); }
strcpy(alphabet, "abcdefghijklmnopqrstuvwxyz");

while ((ch = getopt(argc, argv, "s:e:a:")) != -1)
	{
	switch (ch)
		{
	case 's':
		minLength = strtoumax(optarg, NULL, 0);
		break;
	case 'e':
		maxLength = strtoumax(optarg, NULL, 0);
		break;
	case 'a':
		alphabet = malloc(strlen(optarg) + 1);	// argv[] are read-only
		if (!alphabet) { perror(NULL); exit(1); }
		strcpy(alphabet, optarg);
		break;
	default:
		usage(me, 1);
		break;
		}
	}
argc -= optind;
argv += optind;

if (minLength > maxLength) usage(me, 1);
if (strlen(alphabet) == 0) usage(me, 1);

// setup alphabet and next[]
for (int i = 0; i < strlen(alphabet); i++) alphabet[i] = toupper(alphabet[i]);
for (int i = 0; i < 256; i++) next[i] = alphabet[0];
for (int i = 0; i < strlen(alphabet); i++) next[(unsigned char) alphabet[i]] = alphabet[(i + 1) % strlen(alphabet)];

for (int a = 0; a < argc; a++)
	{
	found = 0;
	seed = strtoumax(argv[a], NULL, 16);

	for (int l = minLength; l <= maxLength; l++)
		{
		fprintf(stderr, "testing %d characters label\n", l);
		found += try(seed, l);
		}

	if (!found)
		printf("no match found for 0x%08x\n", seed);
	printf("\n");
	}

}
