# Author : Bruno Carvalho

.global frame_build
.global frame_check
.global checksum_to_hex

# int frame_build(int value)
# input value in eax
# output in eax
frame_build:
        pushl %ebp                      # Save old stack frame
        movl  %esp, %ebp                # Set new stack base
        movl 8(%ebp), %eax              # copy the argument in eax
        pushl %ebx

        # add byte 0 and 1
        movzbl %al, %ecx
        movzbl %ah, %edx
        addl   %edx, %ecx

        # get byte 2 and add it
        movl   %eax, %edx
        shrl   $16, %edx
        andl   $0xFF, %edx # clearer (only take byte 2)
        addl   %edx, %ecx

        # % 256 (not necessary but clearer)
        andl   $0xFF, %ecx

        # get the inverse
        notl   %ecx # says '~' so notl seems to be the best

        # move eax bytes left and put checksum in
        shll $8, %eax
        movb %cl, %al

        popl %ebx
        leave                           # Restore stack frame
        ret                             # Return to caller

# int frame_check(int value)
# input value in eax
# output in eax
frame_check:
        pushl %ebp                      # Save old stack frame
        movl  %esp, %ebp                # Set new stack base
        movl 8(%ebp), %eax              # copy the argument in eax
        pushl %ebx

        # add checksum and byte 0
        movzbl %al, %ecx
        movzbl %ah, %edx
        addl   %edx, %ecx

        # get byte 1 and add it
        movl   %eax, %edx
        shrl   $16, %edx
        andl   $0xFF, %edx
        addl   %edx, %ecx

        # get byte 2 and add it
        movl   %eax, %edx
        shrl   $24, %edx
        andl   $0xFF, %edx
        addl   %edx, %ecx

        # % 256
        andl   $0xFF, %ecx

        # substract to see difference and return in eax
        subl  $0xFF, %ecx
        movl  %ecx, %eax

        popl %ebx
        leave                           # Restore stack frame
        ret                             # Return to caller

# int checksum_to_hex(int value)
# input value in eax
# output in eax
checksum_to_hex:
        pushl %ebp                      # Save old stack frame
        movl  %esp, %ebp                # Set new stack base
        movl 8(%ebp), %eax              # copy the argument in eax
        pushl %ebx

        # start building "0x??"
        movl $'0', %ebx
        shll $24, %ebx
        movl $'x', %edx
        shll $16, %edx
        orl  %edx, %ebx

        # put 1 to say we're at first character
        movl $1, %edx

        # take first character
        movl %eax, %ecx
        shrl $4, %ecx
        andl $0xF, %ecx

        # compare and jump depending if letter or number
        cmpl $10, %ecx
        jge  hex_to_letter
        jmp  hex_to_number

second_character:
        # register first character
        orb  %cl, %bh

        # put 2 to say we're at the second character
        movl $2, %edx

        # take second character
        movl %eax, %ecx
        andl $0xF, %ecx

        # compare and jump depending if letter or number
        cmpl $10, %ecx
        jge  hex_to_letter

hex_to_number:
        addl $'0', %ecx
        cmpl $1, %edx
        je   second_character
        jmp  end

hex_to_letter:
        addl $('A' - 10), %ecx
        cmpl $1, %edx
        je   second_character
        jmp  end

end:
        # register second character
        orb %cl, %bl

        # save inside eax
        movl %ebx, %eax

        popl %ebx
        leave                           # Restore stack frame
        ret                             # Return to caller
