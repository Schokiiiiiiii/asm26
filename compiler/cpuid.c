#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint32_t cpuid(uint32_t param) {
#   define CPU_STRING_SIZE (3*4+1)

    uint32_t a = 0;
    uint32_t b = 0;
    uint32_t c = 0;
    uint32_t d = 0;

    __asm__ ("movl %4, %%eax\n\t"
             "cpuid\n\t"
             "movl %%eax, %0\n\t"
             "movl %%ebx, %1\n\t"
             "movl %%ecx, %2\n\t"
             "movl %%edx, %3\n\t" // Set eax with param value, call cpuid and mov eax-edx to a-d
            : "+m" (a), "+m"(b), "+m"(c), "+m"(d) // Outputs
            : "g" (param) // Inputs
            : "eax", "ebx", "ecx", "edx" // Clobbered regs
    );

    printf("x86 :\n");
    printf("eax = 0x%08x ebx = 0x%08x ecx = 0x%08x edx = 0x%08x\n", a, b, c, d);
    // This returns the CPU's manufacturer ID string – a twelve-character ASCII string stored in EBX, EDX, ECX (in that order).
    char *cpu_string = malloc(CPU_STRING_SIZE);
    for (int i = 0; i < CPU_STRING_SIZE; ++i) {
      cpu_string[i] = 0;
    }
    *((uint32_t *)(cpu_string)) = b;
    *((uint32_t *)(cpu_string+4)) = d;
    *((uint32_t *)(cpu_string+8)) = c;
    printf("CPU ID String : \"%s\"\n", cpu_string);
    free(cpu_string);

    return a;
}