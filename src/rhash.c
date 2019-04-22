/* this file is part of legotool, by xvi */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>



#define K					0x01000193
#define Kp					0x359c449b			// modular inverse of K
#define DEFAULT_HASH_SEED	0x811c9dc5
#define ID_MATCHANY			0xffffffffffffffff	// no collision possible as IDs are 32 bits only


#if 1

typedef struct { uintmax_t key; uintmax_t game; uintmax_t saveItemID; uintmax_t flags; char *label; char *string; char *redirection; } mapping;

// wordHash was moved to a separate compile entity to save compile time...
extern mapping wordHash[];
extern int wordHashCount;

#else

#include "wordHash.c"

#endif

// default: try 1 & 2 words combinations
// leftmost word is not computed but looked up in wordHash[].
// 2 words combinations sample: with 1,100,000+ words in wordHash[], takes < 10 s
// 3 words combinations are completely unrealistic with 1,000,000+ words in wordHash[]...
// output should be sorted as it will be preceded by its length (most probable matches are shorter)
// e.g. finding label "NumberOfItems" was ridiculously simple once the result was sorted
int tryWords = (1 << 0) | (1 << 1); // that's 1 and 2 words. (1 << 2) stands for 3 words but this takes a huge time O(wordHashCount^2) especially when dictionary has grown large
int trySeparators = 0;
int compareMode = 0;
uintmax_t prefixSeed = DEFAULT_HASH_SEED;
char *prefixLabel = "";
char *suffixLabel = "";
char *separatorLabel = "";


// other globals
static char upper[256];	// using upper[] instead of toupper() for performance



int hashSizeCompare(const void *h1, const void *h2)
{
return strlen(((mapping *) h1)->label) - strlen(((mapping *) h2)->label);
}



int hashFullCompare(const void *h1, const void *h2)
{
uintmax_t k1 = ((mapping *) h1)->key;
uintmax_t k2 = ((mapping *) h2)->key;

if (k1 < k2) return -1;
if (k1 > k2) return +1;

return 0;
}



mapping *hashFullFind(uint32_t h, uintmax_t fv, uintmax_t ch)
{
mapping key;

key.key = h;
key.game = fv;
key.saveItemID = ch;
return bsearch(&key, wordHash, wordHashCount, sizeof(*wordHash), hashFullCompare);
}



uint32_t FNV1UppercaseStringHash32(uint32_t seed, const char *data)
{
uint8_t *ptr = (uint8_t *) data;

while (*ptr)
	seed = (seed * K) ^ toupper(*ptr++);

return seed;
}



int tryCompare(uint32_t hash1, uint32_t hash2)
{
char *w1,*w2;
uint32_t hashA, hashB;
static mapping *wordHash2;
mapping *foundHash;
int result;
static int inited = 0;

if (hash1 == hash2)
	{
	printf("Are you kidding????\n");
	printf("     0 0x%08jx is (0x%08jx, 0x%08jx) without trailing ('', '')\n", (uintmax_t) hash1, (uintmax_t) hash1, (uintmax_t) hash2);
	return 1;
	}

if (!inited)
	{
	// create wordHash2, set up wordHash then sort it
	wordHash2 = malloc(wordHashCount * sizeof(mapping));
	if (wordHash2 == NULL) { perror(NULL); exit(1); }

	memcpy(wordHash2, wordHash, wordHashCount * sizeof(mapping));

	for (uintmax_t i = 0; i < wordHashCount; i++)
		{
		w1 = wordHash[i].label;
	
		hashA = hash1;
		for (int j = strlen(w1) - 1; j >= 0; j--)
			{
			hashA ^= upper[(unsigned char) w1[j]];
			hashA = hashA * Kp;
			}
		wordHash[i].key = hashA;
		}

	qsort(wordHash, wordHashCount, sizeof(*wordHash), hashFullCompare); // wordHash2 has no need to be sorted

	inited = 1;
	}

// now set up wordHash2
for (uintmax_t i = 0; i < wordHashCount; i++)
	{
	w2 = wordHash2[i].label;

	hashB = hash2;
	for (int j = strlen(w2) - 1; j >= 0; j--)
		{
		hashB ^= upper[(unsigned char) w2[j]];
		hashB = hashB * Kp;
		}
	wordHash2[i].key = hashB;
	}
// wordHash2 has no need to be sorted

// now we are setup, go.
result = 0;
for (uintmax_t i = 0; i < wordHashCount; i++)
	{
	hashB = wordHash2[i].key;	// hashFullFind searches wordHash, not wordHash2, so we loop over wordHash2
	w2 = wordHash2[i].label;

	foundHash = hashFullFind(hashB, -1, -1);
	if (foundHash)
		{
		hashA = foundHash->key;
		w1 = foundHash->label;
		if (hashA == 0x811c9dc5)
			printf("%6lu 0x%08jx is (0x%08jx, 0x%08jx) without trailing ('%s', '%s')\n", (uintmax_t)0, (uintmax_t) hashA, (uintmax_t) hash1, (uintmax_t) hash2, w1, w2);
		else
			printf("%6lu 0x%08jx is (0x%08jx, 0x%08jx) without trailing ('%s', '%s')\n", strlen(w1)*1000+strlen(w2), (uintmax_t) hashA, (uintmax_t) hash1, (uintmax_t) hash2, w1, w2);
		result++;
		}
	}

return result;
}



int try(uint32_t seed, uint32_t seedWithSuffix, char *separator)
{
int w2, w3;
uint32_t hash2, hash3;
char *word;
int i, j;
mapping *foundHash;
int result;
char separatorString[256];


j = 0;
for (i = 0; i < strlen(separator); i++)
	{
	switch(separator[i])
		{
	case '\\':
		separatorString[j++] = '\\';
		separatorString[j++] = separator[i];
		break;
	default:
		separatorString[j++] = separator[i];
		break;
		}
	}
separatorString[j] = 0;

// 3 words: we'll try xxx/w3/w2, / being a separator
// 2 words: we'll try xxx/w2, / being a separator
// 1 word:  we'll try xxx, no separator
// xxx is not computed, but searched in dictionary
result = 0;

// 1 word
if (tryWords & 0x1)
	{
	if ((foundHash = hashFullFind(seed, -1, -1)))
		{
		result++;
		printf("%3lu ", (unsigned long) 0); // mark as a perfect match
		printf("{ 0x%08x, gameAny, -1, 0x0, \"%s%s%s\", \"\", NULL },\n", seedWithSuffix, prefixLabel, foundHash->label, suffixLabel);
		}
	}


if (tryWords >= 0x2)
	{
	for (w2 = 0; w2 < wordHashCount; w2++)
		{
		word = wordHash[w2].label;

		if (!word || (word[0] == 0)) continue;	// skip empty labels

		hash2 = seed;

		for (i = strlen(word) - 1; i >= 0; i--)
			{
			hash2 ^= upper[(unsigned char) word[i]];
			hash2 = hash2 * Kp;
			}

		for (i = strlen(separator) - 1; i >= 0; i--)
			{
			hash2 ^= upper[(unsigned char) separator[i]];
			hash2 = hash2 * Kp;
			}

		if ((tryWords & 0x2) && (foundHash = hashFullFind(hash2, -1, -1)))
			{
			if (foundHash->label && foundHash->label[0])
				{
				result++;
				printf("%3lu ", strlen(prefixLabel) + strlen(foundHash->label) + strlen(separator) + strlen(wordHash[w2].label));
				printf("{ 0x%08x, gameAny, -1, 0x0, \"%s%s%s%s%s\", \"\", NULL },\n", seedWithSuffix, prefixLabel, foundHash->label, separatorString, wordHash[w2].label, suffixLabel);
				}
			}

		if (tryWords >= 0x4)
			{
			// try 3rd word
			for (w3 = 0; w3 < wordHashCount; w3++)
				{
				word = wordHash[w3].label;

				if (!word || (word[0] == 0)) continue;	// skip empty labels

				hash3 = hash2;

				for (i = strlen(word) - 1; i >= 0; i--)
					{
					hash3 ^= upper[(unsigned char) word[i]];
					hash3 = hash3 * Kp;
					}

				for (i = strlen(separator) - 1; i >= 0; i--)
					{
					hash3 ^= upper[(unsigned char) separator[i]];
					hash3 = hash3 * Kp;
					}

				if ((tryWords & 0x4) && (foundHash = hashFullFind(hash3, -1, -1)))
					{
					if (foundHash->label && foundHash->label[0])
						{
						result++;
						printf("%3lu ", strlen(prefixLabel) + strlen(foundHash->label) + strlen(separator) + strlen(wordHash[w3].label) + strlen(separator) + strlen(wordHash[w2].label));
						printf("{ 0x%08x, gameAny, -1, 0x0, \"%s%s%s%s%s%s%s\", \"\", NULL },\n", seedWithSuffix, prefixLabel, foundHash->label, separatorString, wordHash[w3].label, separatorString, wordHash[w2].label, suffixLabel);
						}
					}
				}
			}
		}
	}

return result;
}



void usage(char *me, int exitCode)
{
fprintf(stderr, "usage : %s [options] hash\n", me);
fprintf(stderr, "         or\n");
fprintf(stderr, "usage : %s -k hash1 hash2\n", me);
fprintf(stderr, "       dictionary based FNV1 hash reversal\n");
fprintf(stderr, "       options:\n");
fprintf(stderr, "       -1               toggle 1 word\n");
fprintf(stderr, "       -2               toggle 2 words\n");
fprintf(stderr, "       -3               toggle 3 words (unbearably long)\n");
fprintf(stderr, "       -s               separators are tried\n");
fprintf(stderr, "       -S string        string used as separator\n");
fprintf(stderr, "       -r suffix        suffix string for all candidates\n");
fprintf(stderr, "       -p prefix        prefix string for all candidates\n");
fprintf(stderr, "       -P prefix        prefix hash for all candidates\n");
fprintf(stderr, "       -l length        limit dictionary to strings not longer than given length\n");
fprintf(stderr, "       -k hash1 [hash2]...  compare mode\n");
fprintf(stderr, "                        search strings (s1, s2) that, once removed from (hash1, hash2), yield the same hash.\n");
fprintf(stderr, "\n");
fprintf(stderr, "             default is: 1 and 2 words, no separator, prefix empty\n");
fprintf(stderr, "\n");
fprintf(stderr, "             when multiple results are given, they are preceded by a number based on the length, as it is\n");
fprintf(stderr, "               intended to be piped to a 'sort -fu' so that shorter strings appear topmost.\n");
if (exitCode) exit(exitCode);
}



int main(int argc, char **argv)
{
uint32_t seed;
uint32_t seedWithoutSuffix;
int found;
int ch;
char *me = argv[0];
int wordSizeFirstTime = 1;
uint32_t hash1;
uint32_t hash2;
uintmax_t maxLength = UINTMAX_MAX;


// setup upper[]
for (int i = 0; i < sizeof(upper) / sizeof(*upper); i++) upper[i] = i;
for (int i = 'a'; i <= 'z'; i++) upper[i] = toupper(i);

if (argc < 2) return 1;

while ((ch = getopt(argc, argv, "123sS:p:P:k:l:r:")) != -1)
	{
	switch (ch)
		{
	case '1':
		if (wordSizeFirstTime) { wordSizeFirstTime = 0; tryWords = 0; }
		tryWords |= 0x1;
		break;
	case '2':
		if (wordSizeFirstTime) { wordSizeFirstTime = 0; tryWords = 0; }
		tryWords |= 0x2;
		break;
	case '3':
		if (wordSizeFirstTime) { wordSizeFirstTime = 0; tryWords = 0; }
		tryWords |= 0x4;
		break;
	case 's':
		trySeparators = !trySeparators;
		break;
	case 'S':
		separatorLabel = optarg;
		break;
	case 'p':
		prefixLabel = optarg;
		prefixSeed = FNV1UppercaseStringHash32(DEFAULT_HASH_SEED, prefixLabel);
		break;
	case 'r':
		suffixLabel = optarg;
		break;
	case 'P':
		prefixSeed = strtoumax(optarg, NULL, 16);
		prefixLabel = "...";
		break;
	case 'k':
		compareMode = 1;
		hash1 = strtoumax(optarg, NULL, 16);
		break;
	case 'l':
		maxLength = strtoumax(optarg, NULL, 16);
		break;
	default:
		usage(me, 1);
		break;
		}
	}
argc -= optind;
argv += optind;

if (prefixSeed != DEFAULT_HASH_SEED)
	{
	for (uintmax_t i = 0; i < wordHashCount; i++)
		wordHash[i].key = FNV1UppercaseStringHash32(prefixSeed, wordHash[i].label);
	}

if (maxLength < UINTMAX_MAX)
	{
	uintmax_t lower, current, over, length;

	qsort(wordHash, wordHashCount, sizeof(*wordHash), hashSizeCompare);
	lower = 0;
	over = wordHashCount;
	current = over / 2;
	while (current < over)
		{
		length = strlen(wordHash[current].label);
// printf("%ju %ju %ju - %ju\n", lower, current, over, length);
		if (length > maxLength) { over = current; }
		else if (length <= maxLength) { lower = current + 1; }
		current = lower + (over - lower) / 2;
		}
// printf("limit %ju, length %ju %ju %ju\n", current, strlen(wordHash[current - 1].label), strlen(wordHash[current].label), strlen(wordHash[current +1].label));
	fprintf(stderr, "there are %ju words with up to %ju chars\n", current, maxLength);
	wordHashCount = current;
	}

//for (uintmax_t i = 0; i < wordHashCount; i++) wordHash[i].game = wordHash[i].saveItemID = ID_MATCHANY;
qsort(wordHash, wordHashCount, sizeof(*wordHash), hashFullCompare);

if (compareMode)
	{
	if (argc < 1) usage(me, 1);
	if (suffixLabel[0]) usage(me, 1);	// no suffix for now

	for (int a = 0; a < argc; a++)
		{
		hash2 = strtoumax(argv[a], NULL, 16);
		found = tryCompare(hash1, hash2);
		if (!found)
			printf("no match found for (0x%08x,0x%08x)\n", hash1, hash2);
		if (found) printf("\n");
		}
	}
else
	{
	for (int a = 0; a < argc; a++)
		{
		found = 0;
		seed = strtoumax(argv[a], NULL, 16);

		seedWithoutSuffix = seed;
		for (int i = strlen(suffixLabel) - 1; i >= 0; i--)
			{
			seedWithoutSuffix ^= toupper(suffixLabel[i]);
			seedWithoutSuffix *= Kp;
			}

		found += try(seedWithoutSuffix, seed, "");
		if (separatorLabel[0]) found += try(seedWithoutSuffix, seed, separatorLabel);
		if ((tryWords >= 0x2) && trySeparators)
			{	// some separators are stoopid. I don't care
			found += try(seedWithoutSuffix, seed, "Of");	// helped for "CountOfItems"
			found += try(seedWithoutSuffix, seed, "To");	// helped for "JumpToSystem"
			found += try(seedWithoutSuffix, seed, "\\");	// some paths are separated with '\'
			found += try(seedWithoutSuffix, seed, "/");	// ..but most are separated with '/'
			found += try(seedWithoutSuffix, seed, "_");	// seems necessary to build ADDITIONALCONTENT/2017_NEXOKNIGHTS/ABILITIES/QUESTS/CHARACTERUNLOCKQUESTS/CREATURES/_BINARIES/AXL_NEXOKNIGHTS~AXL_ADDITIONALQUESTS.ABILITY
			found += try(seedWithoutSuffix, seed, "~");	// seems necessary to build ADDITIONALCONTENT/2017_NEXOKNIGHTS/ABILITIES/QUESTS/CHARACTERUNLOCKQUESTS/CREATURES/_BINARIES/AXL_NEXOKNIGHTS~AXL_ADDITIONALQUESTS.ABILITY
			found += try(seedWithoutSuffix, seed, "|");	// e.g. "resource_pool|resource_path"
			found += try(seedWithoutSuffix, seed, ":");	// some label constructed with ':'
			found += try(seedWithoutSuffix, seed, " ");	// some label contain ' '
			}

		if (!found)
			printf("no match found for 0x%08x\n", seed);

		if (found) printf("\n");
		}
	}
}
