@ Author : Eliéva Pignat


@ int to_upper_case(char *string)
@ Address of string in r0
@ The upper case string is copied at the same address
@ Return the number of chars in the new string.
@ The return value is stored in r0

to_upper_case:
    mov   r12, sp
    stmfd sp!, {r4-r12, lr}
    @ r0 contains the address of the string

    @ Copy each char to tmp_string
    mov   r3, #0 @ r3 contains loop index

loop:
    @ Get the next character in r2
    add   r1, r0, r3
    ldrb  r2, [r1]

    @ If char is between 'a' and 'z' convert it to upper case
    @ otherwise simply copy it

    cmp   r2, #ascii_a
    blt   copy
    cmp   r2, #ascii_z
    bgt   copy
    sub   r2, r2, #to_upper_sub

copy:
    @ Get the address of tmp_string and store the byte
    ldr   r4, =tmp_string
    add   r4, r4, r3
    strb  r2, [r4]

    @ Increment the loop index
    add   r3, r3, #1

    cmp   r2, #0
    @ End of string is detected by the null char
    beq   end_string

    cmp   r3, #MAX_STRING_SIZE
    blt   loop

    @ Force null character if the string is too long
    ldr   r4, =tmp_string
    add   r4, r4, #MAX_STRING_SIZE
    sub   r4, r4, #1
    mov   r1, #0
    strb  r1, [r4]

end_string:
    @ Copy arguments of the function on the stack
    ldr   r1, =tmp_string
    ldr   r2, =count

    stmfd sp!, {r0, r1, r2, r3, lr}
    bl    copy_string
    ldmfd sp!, {r0, r1, r2, r3, lr}

    @ Copy the size in r0
    ldr   r0, [r2]

    out:
    ldmfd sp!, {r4-r12, lr}
    mov   sp, r12
    mov   pc, lr

