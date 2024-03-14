/* File: palindrome.c 
   Author: Daniel Laritz
   
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> //allows to use "bool" as a boolean type
#include <ctype.h>

/*Optional functions, uncomment the next two lines
 * if you want to create these functions after main: */
bool readLine(char** line, size_t* size, size_t* length) {
    *length = getline(line, size, stdin);
    return true;
}

/* 
  * NOTE that I used char** for the line above... this is a pointer to
  * a char pointer.  I used this because of the availability of
  * a newer function getline which takes 3 arguments (you should look it
  * up) and the first argument is a char**.  You can create a char*, say
  * called var, and to make it a char** just use &var when calling this
  * function.  If this is too confusing, you can use fgets instead.  Feel
  * free to change the function prototypes as you need them.
  * Also, note the use of size_t as a type.  You can look this up, but
  * essentially, this is just a special type of int to track sizes of
  * things like strings...
*/

bool isPalindrome(const char* line, size_t len) {
    if (isalpha(line[i])) { // check if line is alphabetic
        return false;
    }
    // 0:R 1:A 2:C 3:E 4:C 5:A 6:R 7: \n
    for (int i = 0; i < len; i++) {
        if ()
    }
    return false;
}

int main(int argc, char *argv[]) {
    char *line;
    size_t size;
    size_t length;

    while (1) {
        printf("Type a word: ");
        readLine(&line, &size, &length);
        length = getline(&line, &size, stdin);
        printf("%zu characters were read\n", length);
        printf("%zu is the size\n", size);
        printf("You said: %s\n", line);

        if (isPalindrome(line, length)) {
            printf("\"%s\" is a palindrome", word);
        } else {
            printf("\"%s\" is not a palindrome", word);
        }
        free(line);
    }
    //while (1) {

        /*
        readLine(&line, &size, &length)
        if (isPalindrome(line, len)) {
            printf("\"%s\" is a palindrome", word);
        } else {
            printf("\"%s\" is not a palindrome", word);
        }
         */
    //}
    // Cleanup
    return 0;
}
