.text
.extern main
.global _start

_start:
    mov r0, #0 // argc = 0
    mov r1, #0 // argv = NULL
    bl  main   // call main