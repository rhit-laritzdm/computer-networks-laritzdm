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
bool readLine(char** line, size_t* size, size_t* length) {
    while (1) {
        size_t len = getline(line, size, stdin);

        if(len == -1)
            return false;
        // dereferencing line
        // if ends in new line, decrease length by 1 and set it to be null terminator
        if((*line)[len-1] == '\n') {
            len--;
            (*line)[len] = '\0';
        }


        *length = len;

        if(len == 0)
            continue;

        // **line is first character of the array
        // returns true if more than 1 character, or first character is not "."
        return len > 1 || **line != '.';
    }
    /*
    // dereference to assign value not address
    size_t len = getline(line, size, stdin);
    if (*length == 0) {
        return false;
    }
    return true;
     */
}

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
    // Initialize counters to first and last characters
    int i = 0;
    int j = len - 1;
    while (i <= j) {
        // find next alphabetic character for i and j
        while(!isalpha(line[i]) ) {
            i++;
        }
        while(!isalpha(line[j]) ) {
            j--;
        }
        // Check if characters are equal
        if (tolower(line[i]) != tolower(line[j])) {
            return false; // Not a palindrome
        }
        // increment i and decrement j
        i++;
        j--;
    }
    // made it out of while loop, must be a palindrome
    return true;
}

int main(int argc, char *argv[]) {
    if (argc != 1) {
        printf("Usage: ./palindrome\n");
        return 1;
    }
    // ptr -> ptr -> actual array
    char* line;
    size_t size;
    size_t length;
    while(1) {
        printf("Enter a word: ");
        if (!readLine(&line, &size, &length)) {
            return 0;
        }
        //line[strcspn(line, "\n")] = 0; // Remove new line character
        if (isPalindrome(line, length)) {
            printf("\"%s\" is a palindrome.\n", line);
        } else {
            printf("\"%s\" is not a palindrome.\n", line);
        }
    }
}
