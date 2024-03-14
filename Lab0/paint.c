/* File: paint.c 
   Author: Daniel Laritz
   
*/
#define CAN_COVERAGE 200

#include <stdio.h>
#include <math.h>

/* If you do not use the Makefile provided and use gcc,
 * and if you continue to use the math.h library, you
 * will need to include -lm in your gcc compile statement
 * to load the math library */

/* Optional functions, uncomment the next two lines
 * if you want to create these functions after main: */
float readDimension(const char* name) {
    float dimension;
    printf("%s", name);
    scanf("%f", &dimension);
    return dimension;
}

float calcArea(float width, float height, float depth) {
    return 2 * width * height + 2 * width * depth + 2 * height * depth;
}

int main(int argc, char *argv[]){
    //your code here
    float width = readDimension("width> ");
    float height = readDimension("height> ");
    float depth = readDimension("depth> ");
    float area = calcArea(width, height, depth);
    printf("Total Area: %f\n", area);
    int num_cans = ceil((float)area / CAN_COVERAGE);
    printf("Number of cans required: %d\n", num_cans);
    return 0;
}
