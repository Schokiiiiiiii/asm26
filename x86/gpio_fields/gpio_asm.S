# Author : Eliéva Pignat

.global asm_config_register

.equiv GPIO_MODE_MASK,   (0x3 << 0)
.equiv GPIO_SPEED_MASK,  (0xF << 2)
.equiv GPIO_PULLUP_MASK, (0x1 << 6)
.equiv GPIO_ALT_MASK,    (0x7 << 8)
.equiv GPIO_EN_MASK,     (0x1 << 15)
.equiv GPIO_CLEAR_MASK,  (GPIO_MODE_MASK | GPIO_SPEED_MASK | GPIO_PULLUP_MASK | GPIO_ALT_MASK | GPIO_EN_MASK)

# int asm_config_register(int gpio_reg)
# input gpio_reg in eax
# output in eax
asm_config_register:
        pushl   %ebp                # Save old stack frame
        movl    %esp   , %ebp      # Set new stack base
        movl    8(%ebp), %eax      # copy the argument in eax
        pushl   %ebx

        # clear register for bits
        andl    $~GPIO_CLEAR_MASK, %eax

        # set fields
        orl     $(0x2 << 0) , %eax
        orl     $(0x9 << 2) , %eax
        orl     $(0x1 << 6) , %eax
        orl     $(0x5 << 8) , %eax
        orl     $(0x1 << 15), %eax

exit:
        popl    %ebx
        leave                     # Restore stack frame
        ret                       # Return to caller
