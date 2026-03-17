# Author : Eliéva Pignat

.global asm_config_register

# int asm_config_register(int gpio_reg)
# input gpio_reg in eax
# output in eax
asm_config_register:
        pushl %ebp                # Save old stack frame
        movl  %esp, %ebp          # Set new stack base
        movl  8(%ebp), %eax       # copy the argument in eax
        pushl %ebx

        #TODO

exit:
        popl  %ebx
        leave                     # Restore stack frame
        ret                       # Return to caller
