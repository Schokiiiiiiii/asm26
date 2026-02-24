/*
 * exo2.c - Manipulation d'opérateurs logiques
 *
 * Author(s) : Name Surname, Name Surname
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <libleds.h>
#include "tests.h"

uint32_t inverse(uint32_t v){
	// inverse bits with XOR (0^1=1 / 1^1=0)
    return v ^ 0xFFFFFF;
}


int main(int argc, char **argv) {
    init_reg_leds();

    uint32_t leds = get_reg_leds();

    leds = inverse(leds);

    set_reg_leds(leds);

    tests_exo2();
    return 0;
}
