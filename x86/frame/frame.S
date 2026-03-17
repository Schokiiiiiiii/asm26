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

        #TODO

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

        #TODO

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

        #TODO

        popl %ebx
        leave                           # Restore stack frame
        ret                             # Return to caller
