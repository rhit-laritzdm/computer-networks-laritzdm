#include <stdio.h>
#include <stdlib.h>

int main() {
    /*
    char *buffer;
    size_t bufsize = 32;
    size_t characters;

    buffer = (char *)malloc(bufsize * sizeof(char));
    if( buffer == NULL)
    {
        perror("Unable to allocate buffer");
        exit(1);
    }

    printf("Type something: ");
    characters = getline(&buffer,&bufsize,stdin);
    printf("%zu characters were read.\n",characters);
    printf("You typed: '%s'\n",buffer);

    return(0);
     */
    char *input = NULL;     /* input buffer, NULL forces getline to allocate */
    size_t n = 0;           /* maximum characters to read (0 - no limit      */
    ssize_t nchr = 0;       /* number of characters actually read            */

    if ((nchr = getline (&input, &n, stdin)) != -1)
        input[--nchr] = 0;  /* strip newline */

    printf ("\n %s [%zd]\n\n", input, nchr);
    free(input);

    return 0;
}