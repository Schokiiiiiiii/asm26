# Authors : Fabien Léger
# File    : asm_filter.S
# Date    : 10.05.2026
# AT&T Syntax

#include "image_processing.h"

.globl asm_filter
.type asm_filter, @function

# Macro pour check border et jump
.macro jmp_if_border goal, cur
    cmpl \goal, \cur
    je   copy_pixel
.endm

.text
        # Function arguments :
        # ebp + 8  : address of source image data (pointer to unsigned 8 bits)
        # ebp + 12 : address of destination image data (already allocated)
        # ebp + 16 : width of the image (signed 32 bits)
        # ebp + 20 : height of the image (signed 32 bits)
        # ebp + 24 : x position of pixel (signed 32 bits)
        # ebp + 28 : y position of pixel (signed 32 bits)
        #
        # x is the column position
        # y is the row position
        # idk who thought that way but cringe af

asm_filter:
        pushl %ebp                      # Save old stack frame
        movl  %esp, %ebp                # Set new stack base
        pushl %esi                      # Save registers
        pushl %edi                      # Save registers
        pushl %ebx                      # Save registers

        # Copy of the arguments for easier access
        movl 8(%ebp),   %esi            # esi is for source
        movl 12(%ebp),  %edi            # edi is for destination

        movl 16(%ebp),  %eax
        movl %eax,      (width)         # width holds the width parameter
        movl 20(%ebp),  %eax
        movl %eax,      (height)        # height holds the height parameter
        movl 24(%ebp),  %eax
        movl %eax,      (x)             # x holds the x position
        movl 28(%ebp),  %eax
        movl %eax,      (y)             # y holds the y position

        jmp laplace_filter

check_if_on_border:

        # TODO : Check if the current pixel is on the border of the image
        # when on border jump to copy_pixel else continue
        jmp_if_border   $0, (x)             # check x left

        jmp_if_border   $0, (y)             # check y up

        movl            width, %ebx         # get width
        decl            %ebx                # width - 1
        jmp_if_border   %ebx, (x)           # check x right

        movl            height, %ebx        # get height
        decl            %ebx                # height - 1
        jmp_if_border   %ebx, (y)           # check y bottom
        jmp             pixel_not_on_border # no jumps yet means not on a border

laplace_filter:

        # TODO : Implement the Laplace filter
        # retrieve [0, 0]
        movl    y, %ecx                 # retrieve y
        imull   width, %ecx             # multiply it by width
        addl    x, %ecx                 # add x
        movzbl  (%esi, %ecx, 1), %eax   # esi + ecx*1 = image + width*y + x

        movl %eax, (pixel)              # save pixel
        movl %ecx, (raw_position)       # save position inside image
        movl    %eax, (res)             # put pixel in res in case on border

        jmp     check_if_on_border      # jump to check if pixel is on border

pixel_not_on_border:
        movl $0, %eax               # accumulator

        # top pixel
        movl    y, %ecx                 # retrieve y
        decl    %ecx                    # y - 1
        imull   width, %ecx             # multiply it by width
        addl    x, %ecx                 # add x
        movzbl  (%esi, %ecx, 1), %edx   # retrieve top pixel
        addl    %edx, %eax              # add it to accumulator

        # left pixel
        movl    y, %ecx                 # retrieve y
        imull   width, %ecx             # multiply it by width
        addl    x, %ecx                 # add x
        decl    %ecx                    # x - 1
        movzbl  (%esi, %ecx, 1), %edx   # retrieve left pixel
        addl    %edx, %eax              # add it to accumulator

        # bottom pixel
        movl    y, %ecx                 # retrieve y
        incl    %ecx                    # y + 1
        imull   width, %ecx             # multiply it by width
        addl    x, %ecx                 # add x
        movzbl  (%esi, %ecx, 1), %edx   # retrieve bottom pixel
        addl    %edx, %eax              # add it to accumulator

        # right pixel
        movl    y, %ecx                 # retrieve y
        imull   width, %ecx             # multiply it by width
        addl    x, %ecx                 # add x
        incl    %ecx                    # x + 1
        movzbl  (%esi, %ecx, 1), %edx   # retrieve right pixel
        addl    %edx, %eax              # add it to accumulator

        # middle pixel
        movl    pixel, %ecx     # retrieve pixel
        sall    $2, %ecx        # multiply by 4 (2²)
        negl    %ecx            # multiply by -1
        addl    %ecx, %eax      # add it to accumulator

        # Check accumulator state and modify it
        movl    %eax, (res)     # store it inside res
        cmpl    $0, (res)       # check if negative
        jl      negative        # go to negative if res < 0
check_overflow:
        cmpl    $0xFF, (res)    # check if overflow
        jle     copy_pixel      # go to copy if res <= 0xFF
        movl    $0xFF, (res)    # fix res to 0xFF (max)
        jmp     copy_pixel      # go to copy

negative:
        negl    (res)           # res = -res
        jmp     check_overflow  # go to check for overflow

exit:
        popl %ebx                       # Restore registers
        popl %edi       	            # Restore registers
        popl %esi                       # Restore registers
        leave                           # Restore stack frame
        ret                             # Return to caller

copy_pixel:
        # TODO : Copy pixel from source to destination image
        movl    (raw_position), %ecx        # retrieve raw_position
        movl    (res), %eax                 # retrieve res
        movb    %al, (%edi, %ecx, 1)        # copy res into destination image
        jmp     exit                        # exit

.bss
        # Block Started by Symbol Section
        # space is 4 bytes because the values are 32 bit

width:                                  # image width
        .space 4
height:                                 # image height
        .space 4
x:                                      # x position of pixel
        .space 4
y:                                      # y position of pixel
        .space 4
res:                                    # result of convolution
        .space 4
pixel:                                  # pixel value
        .space 4
raw_position:                           # positon x * width + y
        .space 4
