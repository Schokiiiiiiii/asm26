#ifndef __QEMU_BARE__
#define __QEMU_BARE__ 0
#endif

#if __QEMU_BARE__
#   include <common.h>
#   include <exports.h>
#else
#   include <stdint.h>
#   include <stdio.h>
#   include <stdlib.h>
#   include <string.h>
#endif

#include "lscpu-arm.h"

uint32_t cpuid(uint32_t param) {

// https://sourceforge.net/p/predef/wiki/Architectures/
#if __amd64__ || __x86_64__ || _M_AMD64 || _M_IX86 || __i386__
#   define CPU_STRING_SIZE (3*4+1)

    uint32_t a = 0;
    uint32_t b = 0;
    uint32_t c = 0;
    uint32_t d = 0;

    /* Code noyau linux :
    https://elixir.bootlin.com/linux/v5.9.1/source/arch/x86/include/asm/processor.h#L211
    */
    // https://en.wikipedia.org/wiki/CPUID
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

#elif __arm__
    /* Code source noyau linux :
    https://elixir.bootlin.com/linux/latest/source/arch/arm/include/asm/cputype.h
    */

    /*
    Bits	Field	Function
    [31:24] Implementor - Indicates the implementor, ARM:0x41.

    [23:20] Variant - Indicates the variant number, or major revision, of the processor:0x3.

    [19:16] Architecture - Indicates that the architecture is given in the feature registers:0xF.

    [15:4] Primary part number - Indicates the part number, Cortex-A8:0xC08.

    [3:0] Revision - Indicates the revision number, or minor revision, of the processor:0x2.
    */
    uint32_t arm_cpuid;
    // https://developer.arm.com/documentation/ddi0344/k/system-control-coprocessor/system-control-coprocessor-registers/c0--main-id-register?lang=en
    __asm__ ("mrc p15, 0, %0, c0, c0, 0" // Read co-processor p15 with cpopcode1=0 to dest="arm_cpuid" with cpsource=c0, cpred=c0, cpopcode2=0
        : "=r"(arm_cpuid) // Output
    );
    printf("ARM\n");
    printf("0x%08x\n", arm_cpuid);

    uint8_t implementer = arm_cpuid >> 24;
    uint32_t i = 0;
    while ((hw_implementer[i].id != implementer) && (hw_implementer[i].id != -1)) {
      i++;
    }
    printf("Implementer  : %s\n", hw_implementer[i].name);

    //uint8_t variant = (arm_cpuid >> 20) & 0xF;;
    //uint8_t architecture = (arm_cpuid >> 16) & 0xF;

    uint16_t primary_part_number = (arm_cpuid >> 4) & 0xFFF;
    i = 0;
    while ((arm_part[i].id != primary_part_number) && (arm_part[i].id != -1)) {
      i++;
    }
    printf("Primary part : %s\n", arm_part[i].name);

    //uint8_t revision = arm_cpuid & 0xf;

    return arm_cpuid;

#else
#   warning("Unknown processor")

    printf("Unknown processor\n");
    return -1;

#endif

    return 0;
}

int main(int argc, char *argv[]) {

    cpuid(0);

    return 0;
}
