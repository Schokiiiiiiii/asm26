/**
 * @file   backtrace.c
 * @author Rick Wertenbroek
 * @date   25.04.22
 *
 * @brief  Backtrace
 */

#include <common.h>
#include <exports.h>

void function_A(void);
void function_B(void);
void function_C(void);
void function_D(void);
extern void show_backtrace(void);
static uint16_t _rand(void);

#define JUMP_TO_RANDOM_FUNCTION_OR_PRINT_BT() do { \
    printf("Function %s called\n", __FUNCTION__); \
    uint16_t r = _rand() & 0xF; \
    /*printf("Random value %d\n", r);*/ \
    if (r == 0) { \
        show_backtrace(); \
    } else { \
        switch (r & 3) { \
            case 0 : \
                function_A(); \
                break; \
            case 1 : \
                function_B(); \
                break; \
            case 2 : \
                function_C(); \
                break; \
            case 3 : \
                function_D(); \
                break; \
        } \
    } \
} while (0)

static uint16_t lfsr_state;

/// @brief LFSR as pseudo-random number generator (https://gist.github.com/rcc/380978)
const uint16_t lfsr_taps12[] = {0xFFF, (1 << 11), (1 << 5), (1 << 3), (1 << 0), 0};
uint16_t lfsr_inc_16(const uint16_t *taps, uint16_t *lfsr) {
	uint16_t tap = 0;
	int i = 1;

	while(taps[i])
		tap ^= !!(taps[i++] & *lfsr);
	*lfsr <<= 1;
	*lfsr |= tap;
	*lfsr &= taps[0];

	return *lfsr;
}

/// @brief sets its argument as the seed for a new sequence of pseudo-random integers to be returned by rand()
static void _srand(uint16_t seed) {
    if (!seed) {
        seed++;
    }
    lfsr_state = seed;
}

/// @brief function returns a pseudo-random integer
static uint16_t _rand() {
    return lfsr_inc_16(lfsr_taps12, &lfsr_state) - 1;
}

void function_A() {
    JUMP_TO_RANDOM_FUNCTION_OR_PRINT_BT();
}

void function_B() {
    JUMP_TO_RANDOM_FUNCTION_OR_PRINT_BT();
}

void function_C() {
    JUMP_TO_RANDOM_FUNCTION_OR_PRINT_BT();
}

void function_D() {
    JUMP_TO_RANDOM_FUNCTION_OR_PRINT_BT();
}

/// @brief The main entrypoint of the application
int main(int argc, char *argv[]) {
    int err = 0;
    int seed = 0;

    printf("This program will jump randomly between functions A, B, C or D\n");
    printf("Please press a key to use as random seed value\n");
    printf("----\n");
    
    seed = getc();
    _srand(seed);
    JUMP_TO_RANDOM_FUNCTION_OR_PRINT_BT();

    printf("----\n");
	printf("Hit any key to exit ... ");
	while (!tstc());

	/* consume input */
	(void) getc();

	printf("\n\n");

	return err;
}