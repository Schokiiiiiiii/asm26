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
        movl  8(%ebp), %eax     # eax = start
        pushl %ebx

        ####### Student code below
        movl    $0, %ecx        # counter for number of rooms in the stack

        # start room checks
        cmp     $0, %eax                # check we're not already null
        je      no_more_rooms           # end program if null
        movl    $1, 0x14(%eax)          # put visited as 1

visit_room:
        # SHOW ROOM
        # check room name exists
        cmpl    $0, (%eax)      # if room data equal to null
        je      check_around    # go directly to check around

        # print the name
        PUT_S   (%eax)          # output room
        movb    $0x10, cur_dir  # fix first direction as west

check_around:
        # ADD OTHER ROOMS
        # retrieve current direction
        movzbl  cur_dir, %ebx           # ebx = dir
        movl    (%eax, %ebx), %edx      # edx = dir* (room + dir)

        # check if null
        cmp     $0, %edx        # check if the direction is null
        je      next_direction  # go to the next direction

        # check if visited
        cmp     $1, 0x14(%edx)      # ebx == 1 visited so skip
        je      next_direction      # jump to visited if equal

        # put it as visited (since it's already in stack now)
        movl    $1, 0x14(%edx)          # put visited as 1

        # add it to stack
        pushl   %edx            # push it as a room to visit
        incl    %ecx            # add 1 to nb of rooms to visit

next_direction:
        # update direction
        subb    $4, cur_dir     # go to the next direction
        cmpb    $0, cur_dir     # check if we finished checking directions
        jg      check_around      # jump if not finished

next_room:
        # MOVE TO NEXT ROOM
        cmp     $0, %ecx        # check if more rooms
        je      no_more_rooms   # if no more rooms end program
        popl    %eax            # else get next room
        decl    %ecx            # one less room to visit
        jmp     visit_room      # visit that room

no_more_rooms:
        ######## End of student code

        popl %ebx
        leave
        ret

.data
format_string:
    .string "%s "

cur_dir:
    .byte   0x10