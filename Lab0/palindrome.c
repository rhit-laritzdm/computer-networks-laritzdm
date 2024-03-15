/* File: palindrome.c 
   Author: Daniel Laritz
   
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> //allows to use "bool" as a boolean type
#include <ctype.h>
#include <string.h>

/*Optional functions, uncomment the next two lines
 * if you want to create these functions after main: */

/* 
  * NOTE that I used char** for the line above... this is a pointer to
  * a char pointer.  I used this because of the availability of
  * a newer function getline which takes 3 arguments (you should look it
  * up) and the first argument is a char**.  You can create a char*, say
  * called var, and to make it a char** just use &var when calling this
  * function.  If this is too confusing, you can use fgets instead.  Feel

  * essentially, this is just a special type of int to track sizes of
  * things like strings...
*/

bool isPalindrome(const char* line, size_t len) {
    for (int i = 0; i < len; i++) {
        if (!isalpha(line[i])) {
            return false; // Not alphabetical
        }
        if (line[i] != line[len - i - 1]) {
            return false; // Not a palindrome
        }
    }
    return true;
}

int main(int argc, char *argv[]) {
    while(1) {
        char word[20];
        printf("Enter a word: ");
        fgets(word,20,stdin);
        word[strcspn(word, "\n")] = 0; // Remove new line character
        size_t len = strlen(word);
        if ((len == 1) && (word[0] == '.')) {
            return 0;
        }
        if (isPalindrome(word, len)) {
            printf("\"%s\" is a palindrome.\n", word);
        } else {
            printf("\"%s\" is not a palindrome.\n", word);
        }
    }
}
