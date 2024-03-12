#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void function1(int n) {
    printf("In function1, n=%d\n", n);
    n+=30;
}

void function2(int* n) {
    printf("In function 2, *n = %d\n", *n);
    *n += 100;
}

typedef struct {
    int length;
    char* word;
} Word_tracker;

/* limiting and wasting space by doing word[100]
typedef struct {
    int length;
    char word[100];
} Word_tracker;
*/


// vim inClass.c
// gcc inClass.c -o inclass
// inclass words.dms
int main(int argc, char* argv[]) {
    printf("hello world!\n");
    int a = 10;

    int* p = a;

    int b[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int* bPtr = b;

    printf("b[0] = %d; b[0] = %d; *bPtr = %d; bPtr[1] = %d; *(bPtr + 1) = %d; \n", b[0], b[1], *bPtr, bPtr[1], *(bPtr+1));

    printf("a = %d; *p = %p; &a = %p\n", a, *p, p, &a);

    function1(a);
    function1(p);

    printf("a = %d; *p = %p; &a = %p\n", a, *p, p, &a);

    function2(p);
    printf("a = %d; *p = %p; &a = %p\n", a, *p, p, &a);

    function2(&a); // Address of a
    printf("a = %d; *p = %p; &a = %p\n", a, *p, p, &a);

    /*
    if(argc < 2) {
        printf("Usage: ./program filename\n");
        return 1;
    }

    char* filename = argv[1];

    FILE* fp = fopen(filename, "r"); // if u want to read in binary, use "rb", write binary "wb", write "w"
    // make sure file opens
    if (fp == NULL) {
        printf("File %s not found!\n", filename);
        return 1;
    }

    char curWord[100]; // don't use
    Word_tracker longestWord;
    longestWord.length = 0;
    longestWord.word = NULL;

    while(fscanf(fp, "%s", curWord) == 1) {
        int curLength = strlen(curWord);

        if(curLength > longestWord.length) {
            if(longestWord.word != NULL) {
                free(longestWord.word); // frees space to prevent memory leak
            }

            longestWord.length = curLength;
            longestWord.word = (char*) malloc(curLength*sizeof(char));
            strncpy(longestWord.word, curWord, curLength);   // strcpy is string copy, strncpy is safer
            // need exactly right length array or \0 at end of array content
        }
    }

    fclose(fp);
    printf("The longest word \"%s\" is of size %d\n", longestWord.word, longestWord.length);
    free(longestWord.word);
     */
    return 0;
}