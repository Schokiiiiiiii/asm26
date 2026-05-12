# Author : Prenom Nom

.global traverse_maze_asm

.extern printf

# Macro that will print what is at the end of string_ptr (char *)
# with an extra space at the end.
.macro PUT_S string_ptr
    pushal
    push \string_ptr
    push $format_string
    call printf
    add  $0x8, %esp
    popal
.endm

## @brief void traverse_maze_asm(Room *start);
## Traverses a maze/graph using DFS and prints room data if available.
##
## Room layout:
## +0x00 : data
## +0x04 : north
## +0x08 : east
## +0x0c : south
## +0x10 : west
## +0x14 : visited
##
## @param Room *start the starting room
traverse_maze_asm:
        # Intro
        pushl %ebp
        movl  %esp, %ebp
        movl  8(%ebp), %eax             # eax = start
        pushl %ebx

    ####### Student code below

        
    ######## End of student code

        popl %ebx
        leave
        ret

.data
format_string:
    .string "%s "