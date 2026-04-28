        # Authors :
        # File    : asm_filter.S
        # Date    :
        # AT&T Syntax

#include "image_processing.h"

.globl asm_filter
.type asm_filter, @function

.text
        # Function arguments :
        # ebp + 8  : address of source image data (pointer to unsigned 8 bits)
        # ebp + 12 : address of destination image data (already allocated)
        # ebp + 16 : width of the image (signed 32 bits)
        # ebp + 20 : height of the image (signed 32 bits)
        # ebp + 24 : x position of pixel (signed 32 bits)
        # ebp + 28 : y position of pixel (signed 32 bits)

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

check_if_on_border:

        # TODO : Check if the current pixel is on the border of the image
        # when on border jump to copy_pixel else continue

laplace_filter:

        # TODO : Implement the Laplace filter

exit:
        popl %ebx                       # Restore registers
        popl %edi       	        # Restore registers
	popl %esi                       # Restore registers
	leave                           # Restore stack frame
	ret                             # Return to caller

copy_pixel:

        # TODO : Copy pixel from source to destination image

        jmp exit                        # exit

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
