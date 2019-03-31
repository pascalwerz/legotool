/* this file is part of legotool, by xvi */
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>

#include "fnv.h"
#include "mappedFile.h"


uint32_t checksum(const void *data, uintmax_t size)
{
	return ~FNV1DataHash32(0xffffffff, data, size);
}



void usage(char *me, int exitCode)
{
fprintf(stderr, "usage: %s [options] file\n", me);
fprintf(stderr, "           options:\n");
fprintf(stderr, "       -s startOffset    file offset of first bytes included in checkum, defaults to '0'.\n");
fprintf(stderr, "       -e endOffset      file offset of last bytes included in checkum, defaults to 'last byte of file'.\n");
fprintf(stderr, "       -l length         number of bytes included in checksum, defaults to 'up to end of file'.\n");
fprintf(stderr, "       -r checksum       search file for a single block of one or more bytes that match the given checksum.\n");
fprintf(stderr, "                         with -r, -s indicates the lowest limit for the block start.\n");
fprintf(stderr, "       All of -s, -e and -l values can be input as decimal or as hexadecimal with leading 0x.\n");
fprintf(stderr, "\n");
fprintf(stderr, "           if -s is beyond end of file, an error occurs.\n");
fprintf(stderr, "           -l or -e are mutually exclusive and are truncated to end of file\n");
fprintf(stderr, "           you may give -s without -e nor -l, this will default to 'all the rest of the file'\n");

if (exitCode) exit(exitCode);
}



int main(int argc, char **argv)
{
uintmax_t givenFirst, givenLast, givenLength;
uintmax_t first, last, length;
uintmax_t willTargetChecksum;
uint32_t targetChecksum;
uint32_t targetChecksumComplement;
uint8_t *fileData;
uintmax_t fileDataSize;
mappedfile_t file;
char ch;
char *me;
uint32_t crc;


me = argv[0];
givenFirst = 0;
givenLast = UINTMAX_MAX;
givenLength = UINTMAX_MAX;
willTargetChecksum = 0;
while ((ch = getopt(argc, argv, "e:l:s:r:?")) != -1)
	{
	switch (ch)
		{
	case 's':
		givenFirst = strtoumax(optarg, NULL, 0);
		break;
	case 'e':
		if (givenLength != UINTMAX_MAX) usage(me, 0);
		givenLast = strtoumax(optarg, NULL, 0);
		break;
	case 'l':
		if (givenLast != UINTMAX_MAX) usage(me, 0);
		givenLength = strtoumax(optarg, NULL, 0);
		break;
	case 'r':
		willTargetChecksum = 1;
		targetChecksum = strtoumax(optarg, NULL, 0);
		break;
	case '?':
	default:
		usage(me, 1);
		break;
		}
	}
argc -= optind;
argv += optind;
// remember: remaining args are now in argv[0] and up

if (willTargetChecksum)
	{
	targetChecksumComplement = ~targetChecksum;
	if (givenFirst == UINTMAX_MAX) givenFirst = 0;

	for (int argIndex = 0; argIndex < argc; argIndex++)
		{
		fileData = mapFileAtPath(argv[argIndex], &file, MAPPEDFILE_RDONLY);
		if (fileData == NULL) { perror(argv[argIndex]); exit(2); }
		fileDataSize = file.size;

		if (givenFirst >= fileDataSize)
			{
			fprintf(stderr, "error: start offset beyond end of file '%s'\n", argv[argIndex]);
			exit(1);
			}

		for (first = givenFirst; first < fileDataSize; first++)
			{
			crc = 0xffffffff;
			for (length = 0; length < fileDataSize - first; length++)
				{ // here, length is 1 less than true length
				crc = (crc * FNV_PRIME_32) ^ fileData[first + length];
				if (crc == targetChecksumComplement)
					{
					printf("checksum of bytes [0x%jx..0x%jx] (length 0x%08jx) of file '%s' is: 0x%08jx\n", first, first + length, length + 1, argv[argIndex], (uintmax_t) targetChecksum);
					}
				}
			}

		unmapFile(&file, MAPPEDFILE_DISCARD);
		}
	}
else
	{
	for (int argIndex = 0; argIndex < argc; argIndex++)
		{
		fileData = mapFileAtPath(argv[argIndex], &file, MAPPEDFILE_RDONLY);
		if (fileData == NULL) { perror(argv[argIndex]); exit(2); }
		fileDataSize = file.size;
		first = givenFirst;
		if (first >= fileDataSize)
			{
			fprintf(stderr, "error: start offset beyond end of file '%s'\n", argv[argIndex]);
			exit(1);
			}
		if (givenLength == UINTMAX_MAX)
			{ // givenLast applies
			last = (givenLast >= fileDataSize) ? (fileDataSize - 1) : (givenLast);
			length = last - first + 1;
			}
		else if (givenLast == UINTMAX_MAX)
			{ // givenLength applies
			length = ((givenLength + first) > fileDataSize) ? (fileDataSize - first) : (givenLength);
			}

		crc = checksum(fileData + first, length);
		printf("checksum is: 0x%08jx\n", (uintmax_t) crc);

		unmapFile(&file, MAPPEDFILE_DISCARD);
		}
	}

return 0;
}



