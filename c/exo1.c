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
    
    if (string == NULL)
        return NULL;

    char *alloc = malloc(sizeof(char) * length + 1);
    if (alloc == NULL)
        return NULL;
    
    for (int i = 0 ; i < length ; ++i)
        *(alloc + i) = *(string + i);

    *(alloc + length) = '\0';

    return alloc;
}

int main(int argc, char **argv) {
    printf("Welcome to ASM\n");
    const char *string = "Bonjour de main!";

    char *alloc = allocate_string(string, strlen(string));

    printf("Received allocated message : %s\n", alloc);

    tests_exo1();
    return 0;
}
