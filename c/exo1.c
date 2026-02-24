/*
 * exo1.c - String allocation and copy exercise
 *
 * Author(s) : Name Surname, Name Surname
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "tests.h"

char *allocate_string(const char *string, int length) {
    
	// check string is not NULL
    if (string == NULL)
        return NULL;

    // allocate length size
    char *alloc = malloc(sizeof(char) * length + 1);
    if (alloc == NULL)
        return NULL;
    
    // copy character by character
    for (int i = 0 ; i < length ; ++i)
        *(alloc + i) = *(string + i);

    // add \0 at the end
    *(alloc + length) = '\0';

    // return the pointer
    return alloc;
}

int main(int argc, char **argv) {
	// entrance of the program
    printf("Welcome to ASM\n");

    // create string and allocate a copy
    char string[] = "Bonjour de main!";
    char *alloc = allocate_string(string, strlen(string));

    // print message received
    printf("Received allocated message : %s\n", alloc);

    // test the results
    tests_exo1();
    return 0;
}
