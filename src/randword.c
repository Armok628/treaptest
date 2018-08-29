#include "randword.h"
static const char *vwl="aeiouy";
static const int n_vwl=6;
static const char *cns="bcdfghjklmnpqrstvwxyz";
static const int n_cns=21;
char *random_word(int length)
{
	char *word=calloc(length+1,1);
	int vowel_start=rand()%2;
	for (int i=0;i<length;i++)
		if (vowel_start^(i%2))
			word[i]=vwl[rand()%n_vwl];
		else
			word[i]=cns[rand()%n_cns];
	return word;
}
