@ ASM
@ Author :

@@ @brief void show_backtrace() prints the backtrace up to main()
.global show_backtrace
show_backtrace:
        stmfd   sp!, {r4, r5, fp, lr}
        add     fp, sp, #12

        @@@@@@@@@@@ A completer @@@@@@@@@@

        @ current frame pointer used for walking the stack
        mov     r4, fp

        @ Print title
        ldr     r0, =backtrace_string
        bl      printf

        @ Done by Student
loop_backtrace:
        @ Retrieve lr and old fp
        ldr     r2, [r4, #0]    @ load lr
        ldr     r3, [r4, #-4]   @ load old fp

        @ Check we're not finished
        cmp     r2, #-1
        bne     backtrace_this_shit
        cmp     r3, #-1
        beq     ending_this_shit

backtrace_this_shit:

        @ Print old lr -> printf(r0, r1)
        ldr     r0, =format_string
        mov     r1, r2

        @ Call printf
        stmfd   sp!, {r2, r3}
        bl      printf
        ldmfd   sp!, {r2, r3}

        @ Replace fp by old fp
        mov     r4, r3
        b       loop_backtrace

ending_this_shit:
	    @@@@@@@@@@@ ----------- @@@@@@@@@@
        sub     sp, fp, #12
        ldmfd   sp!, {r4, r5, fp, pc}

.data

backtrace_string:
        .string "\nBacktrace :\n----\n"

format_string:
        .string "Previous link register : 0x%p\n"