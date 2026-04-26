#include <common.h>
#include <exports.h>


uint32_t pop_count_c(const uint32_t value) {
    uint32_t result = 0;

    for (int i = 0 ; i < 32 ; ++i)
        result += (value >> i) & 0x1;

    return result;
}

uint32_t pop_count(const uint32_t value) {
    uint32_t result = 0;

    __asm__("movl   %1, %%eax\n\t"
            "popcnt %%eax, %%ecx\n\t"
            "movl   %%ecx, %0\n\t"
            : "+m" (result) // Outputs
            : "g"  (value)  // Inputs
            : "eax", "ecx"  // Clobbered regs
    );

    return result;
}

void test_popcount(void) {
        printf("%s: Test1: pop_count_c(0)\n", (pop_count_c(0) == 0) ? "OK" : "FAILED");
        printf("%s: Test2: pop_count(0)\n", (pop_count(0) == 0) ? "OK" : "FAILED");
        printf("%s: Test3: pop_count_c(0xFFFFFFFF)\n", (pop_count_c(0xFFFFFFFF) == 32) ? "OK" : "FAILED");
        printf("%s: Test4: pop_count(0xFFFFFFFF)\n", (pop_count(0xFFFFFFFF) == 32) ? "OK" : "FAILED");
        printf("%s: Test5: pop_count_c(0xAAAA5555)\n", (pop_count_c(0xAAAA5555) == 16) ? "OK" : "FAILED");
        printf("%s: Test6: pop_count(0xAAAA5555)\n", (pop_count(0xAAAA5555) == 16) ? "OK" : "FAILED");
}


int main(int argc, char *argv[]) {
    uint32_t input = 0xA000000A;
    uint32_t expected_result = 4;

    uint32_t pop_count_result = pop_count(input);
    uint32_t pop_count_c_result = pop_count_c(input);

    if (pop_count_c_result != expected_result) {
        printf("[C] [KO] Expected result is %d but %d was returned\n", expected_result, pop_count_c_result);
    } else {
        printf("[C] [OK] Correct !\n");
    }

    if (pop_count_result != expected_result) {
        printf("[ASM] [KO] Expected result is %d but %d was returned\n", expected_result, pop_count_result);
    } else {
        printf("[ASM] [OK] Correct !\n");
    }

    printf("Built-in result is %d\n", __builtin_popcount(input));
    test_popcount();

    printf("Hit any key to exit ... ");
    while (!tstc());

    /* consume input */
    (void) getc();

    printf("\n\n");
    return 0;
}
