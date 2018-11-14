/* by xvi */
#include "keywords.h"
#include <stdlib.h>
#include <string.h>
#include <strings.h>



int keywordCaseInsensitiveCompare(const void *k1, const void *k2)
{
return strcasecmp(((keyword_t *)k1)->fullName, ((keyword_t *)k2)->fullName);
}



int keywordCaseSensitiveCompare(const void *k1, const void *k2)
{
return strcmp(((keyword_t *)k1)->fullName, ((keyword_t *)k2)->fullName);
}



int prepareKeywords(keyword_t *keywords, uintmax_t keywordCount, int caseSensitive)
{
int (*compareFunction)(const void *k1, const void *k2);
uintmax_t iLength;
uintmax_t jLength;
uintmax_t minLength;


if (caseSensitive)
	compareFunction = keywordCaseSensitiveCompare;
else
	compareFunction = keywordCaseInsensitiveCompare;

qsort(keywords, keywordCount, sizeof(keyword_t), compareFunction);

for (uintmax_t i = 0; i < keywordCount; i++)
	{
	keywords[i].minimumCharacters = 0;
	keywords[i].fullNameLength = strlen(keywords[i].fullName);
	if ((i + 1 < keywordCount) && !compareFunction(&keywords[i], &keywords[i + 1]))
		return -2; // identical keywords not allowed
	}

for (uintmax_t i = 0; i < keywordCount; i++)
	{
	iLength = keywords[i].fullNameLength;
	if (iLength == 0) return -1; // empty keywords not allowed

	for (uintmax_t j = i + 1; j < keywordCount; j++)
		{
		jLength = strlen(keywords[j].fullName);

		if (iLength < jLength)
			minLength = iLength + 1;
		else
			minLength = jLength + 1;

		for (uintmax_t k = 0; k < minLength; k++)
			{
			if (keywords[i].fullName[k] != keywords[j].fullName[k])
				{
				if ((k + 1) > keywords[i].minimumCharacters)
					{
					if (k < iLength)
						keywords[i].minimumCharacters = k + 1;
					else
						keywords[i].minimumCharacters = iLength;
					}
				if ((k + 1) > keywords[j].minimumCharacters)
					{
					if (k < jLength)
						keywords[j].minimumCharacters = k + 1;
					else
						keywords[j].minimumCharacters = jLength;
					}
				break;
				}
			}
		}
	}

return 0;
}



const keyword_t *keywordLookup(const char *string, const keyword_t *keywords, uintmax_t keywordCount, int caseSensitive)
{
uintmax_t current;
uintmax_t lower;
uintmax_t upper;
uintmax_t stringLength;
int cmp;
int (*compareFunction)(const char *s1, const char *s2);
int (*compareNFunction)(const char *s1, const char *s2, size_t n);


if (caseSensitive)
	{
	compareFunction = strcmp;
	compareNFunction = strncmp;
	}
else
	{
	compareFunction = strcasecmp;
	compareNFunction = strncasecmp;
	}

stringLength = strlen(string);

// locate lowest keyword <= string
lower = 0;
upper = keywordCount;
current = (lower + upper) / 2;
while (lower < upper)
	{
	cmp = compareFunction(string, keywords[current].fullName);
	if (cmp == 0) break;
	else if (cmp < 0) upper = current;
	else lower = current + 1;
	current = (lower + upper) / 2;
	}

if (stringLength < keywords[current].minimumCharacters) return NULL;	// not enough characters to uniquely identify
if (stringLength > keywords[current].fullNameLength) return NULL;		// too much characters to match
if (compareNFunction(string, keywords[current].fullName, stringLength)) return NULL;

return &keywords[current];
}
