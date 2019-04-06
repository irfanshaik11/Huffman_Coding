#ifndef wordfreq_h
#define wordfreq_h

//#include "main.c"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>


#define MAXWORDS    1000
#define MAXSTRING    100

typedef struct _word {
    char    s[MAXSTRING];    /* the word */
    int    count;        /* number of times word occurs */
} word;



int insert_word (word *words,int *n, char *s) {
    if (strcmp(s, "") == 0){return 1; }
    int i;

    /* linear search for the word */
    printf("Inserting word: %s", s);
    
    printf("n is %d", *n);fflush(stdout);
    
    for (i=0; i<=*n; i++) if (strcmp (s, words[i].s) == 0) {

        /* found it?  increment and return. */

        words[i].count++;
        return *n;
    }

    /* error conditions... */

    if (strlen (s) >= MAXSTRING) {
        fprintf (stderr, "word too long!\n");
        exit (1);
    }
    if (*n >= MAXWORDS) {
        fprintf (stderr, "too many words!\n");
        exit (1);
    }

    /* copy the word into the structure at the first available slot,
     * i.e., *n
     */
    
    strcpy(words[*n].s, s);

    words[*n].count = 1;

    /* one more word */

    (*n)++;
    return *n;
}

/* comparison function for quicksort.  this lets quicksort sort words
 * by descending order of count, i.e., from most to least frequent
 */
int wordcmp (word *a, word *b) {
    if (a->count < b->count) return +1;
    if (a->count > b->count) return -1;
    return 0;
}

/* return 1 if c is alphabetic (a..z or A..Z), 0 otherwise */
int is_alpha (char c) {
    if ( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) return 1;
    return 0;
}

/* remove the i'th character from the string s */
void remove_char (char *s, int i) {
    while (s[i]) {
        i++;
        s[i-1] = s[i];
    }
    s[i] = 0;
}

/* remove non-alphabetic characters from the string s */
void remove_non_alpha (char *s) {
    int i;
    
    for (i=0; s[i]; i++) if (!is_alpha (s[i])) remove_char (s, i);
}

/* make all the letters in s lowercase */
void make_lowercase (char *s) {
    int i;
    
    for (i=0; s[i]; i++) s[i] = tolower (s[i]);
}

#endif /* hashmap_h */
