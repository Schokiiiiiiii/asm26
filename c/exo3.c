/*
 * exo3.c - Endianness determination exercise
 *
 * Author(s): Your Name(s)
 */

#include <stdio.h>
#include <stdint.h>
#include "tests.h"

int check_endianness() {
	// create a 16 bits integer and a pointer per byte on it
    uint16_t checker = 0x1234;
    uint8_t *ptr = (uint8_t *)&checker;
    // order of reading => big endian
    if (*ptr == 0x12)
    	return 0; 	// big endian
    return 1;		// little endian
}

int main(int argc, char **argv) {

    tests_exo3();

    if (check_endianness())
        printf("This machine is little-endian.\n");
    else
        printf("This machine is big-endian.\n");
    
    return 0;
}
