/* by xvi */
#ifndef _KEYWORDS_H_
#define _KEYWORDS_H_ 1


#include <stdint.h>


// below functions rely on an array of keyword_t

// if a keyword is the same as another but with additional character(s),
// the longer's minimumCharacters is required to be at least one character more than the shorter,
// and the shorter's minimumCharacters is required to be its full length
typedef struct 
	{
	uintmax_t userData;				// free use, ignored by functions below, intended to numerically identify keyword
	uintmax_t userFlags;			// free use, ignored by functions below, intended to be flags for the keyword
	uintmax_t minimumCharacters;	// minimum chars in fullName required to uniquely differentiate this keyword from other keywords
	uintmax_t fullNameLength;		// which is also maximum chars in fullName required to uniquely identify this keyword
	char *fullName;
	} keyword_t;



// on success, returns 0. fails if there are duplicates or empty keywords
// alternatively, you may prepare the array manually, e.g. with a sorted compile-time list of keywords, minimumCharacters and fullNameLength filled.
int prepareKeywords(keyword_t *keywords, uintmax_t keywordCount, int caseSensitive);

// on success, returns a pointer to identified keyword
// on failure, returns NULL (failure occurs when input string is too short or doesn't match any keyword)
// caseSensitive must match the one given in prepareKeywords() call
const keyword_t *keywordLookup(const char *string, const keyword_t *keywords, uintmax_t keywordCount, int caseSensitive);

#endif // _KEYWORDS_H_
