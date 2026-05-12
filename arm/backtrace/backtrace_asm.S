@ ASM
@ Author :

@@ @brief void show_backtrace() prints the backtrace up to main()
.global show_backtrace
show_backtrace:
        stmfd sp!, {fp, lr}
        add fp, sp, #4

        @@@@@@@@@@@ A completer @@@@@@@@@@

        ldr r0, =backtrace_string
        bl printf

        @ TODO

	@@@@@@@@@@@ ----------- @@@@@@@@@@
        sub sp, fp, #4
        ldmfd sp!, {fp, pc}

.data

backtrace_string:
        .string "\nBacktrace :\n----\n"

format_string:
        .string "Previous link register : 0x%p\n"