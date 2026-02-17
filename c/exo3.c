/*
 * exo3.c - Endianness determination exercise
 *
 * Author(s): Your Name(s)
 */

#include <stdio.h>
#include "tests.h"

int check_endianness() {
    //TODO
}

int main(int argc, char **argv) {

    tests_exo3();

    if (check_endianness())
        printf("This machine is little-endian.\n");
    else
        printf("This machine is big-endian.\n");
    
    return 0;
}