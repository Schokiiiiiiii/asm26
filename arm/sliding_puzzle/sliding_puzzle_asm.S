@ Author : Fabien Léger

.global swap_asm
.global hamming_distance_asm
.global manhattan_distance_asm

@@ @brief void swap_asm(uint8_t *a, uint8_t *b)
@@ Swaps the byte at location a with the byte at location b
@@
@@ @param a (r0) location of first byte
@@ @param b (r1) location of second byte
@@ @return void
swap_asm:
	mov   r12, sp
        stmfd sp!, {r4-r12, lr}

        @@@@@@@@@@@ A completer @@@@@@@@@@

        @ r0 = address of a
        @ r1 = address of b

        ldrb r2, [r0] @ r2 = *a
        ldrb r3, [r1] @ r3 = *b

        strb r3, [r0] @ *a = r3
        strb r2, [r1] @ *b = r2

	    @@@@@@@@@@@ ----------- @@@@@@@@@@

        ldmfd sp!, {r4-r12, lr}
        mov   sp, r12
        mov   pc, lr

@@ @brief uint32_t hamming_distance_asm(const uint8_t *puzzle)
@@ Computes the Hamming distance between the puzzle passed and the goal
@@
@@ Goal :
@@ 1 2 3
@@ 4 5 6
@@ 7 8 0 - 0 denoting the empty tile
@@
@@ @param puzzle (r0) location of first byte of the puzzle
@@ @return the Hamming distance between passed puzzle and the goal in r0
hamming_distance_asm:
	mov   r12, sp
        stmfd sp!, {r4-r12, lr}

        @@@@@@@@@@@ A completer @@@@@@@@@@

        mov r1, #0        @ init sum
        mov r2, #0        @ init loop (i = 0)

loop_hamming:
        ldrb r3, [r0, r2] @ r3 = puzzle[i]

        cmp r2, #8        @ special case if 8 (last case)
        moveq r4, #0      @ mov 0 for null case
        addne r4, r2, #1  @ goal = i + 1

        cmp r3, r4        @ compare goal and current
        addne r1, r1, #1  @ if not equal => add 1

        add r2, r2, #1    @ go forward in the loop
        cmp r2, #9        @ compare with 9
        bne loop_hamming  @ branch until equal to 9

        mov r0, r1        @ mov sum to return register

	    @@@@@@@@@@@ ----------- @@@@@@@@@@

        ldmfd sp!, {r4-r12, lr}
        mov   sp, r12
        mov   pc, lr


@@ @brief uint32_t manhattan_distance_asm(const uint8_t *puzzle)
@@ Computes the Manhattan distance between the puzzle passed and the goal
@@
@@ Goal :
@@ 1 2 3
@@ 4 5 6
@@ 7 8 0 - 0 denoting the empty tile
@@
@@ @param puzzle (r0) location of first byte of the puzzle
@@ @return the Manhattan distance between passed puzzle and the goal in r0
manhattan_distance_asm:
	mov   r12, sp
        stmfd sp!, {r4-r12, lr}

        @@@@@@@@@@@ A completer @@@@@@@@@@

        @@ r0  : index / return value at the end
        @@ r1  : sum
        @@ r2  : loop index
        @@ r3  : puzzle[i] (current tile)
        @@ r4  : target index
        @@ r5  : x_current
        @@ r6  : y_current
        @@ r7  : x_target
        @@ r8  : y_target
        @@ r9  : dx
        @@ r10 : dy

        mov r1, #0        @ sum
        mov r2, #0        @ i

loop_manhattan:
        ldrb r3, [r0, r2] @ r3 = puzzle[i]

        cmp r3, #0        @ special case if 0
        moveq r4, #8      @ target index of empty tile
        subne r4, r3, #1  @ target index = tile value - 1 (1 has index 0)

        @@@@@@@@ FINDING CURRENT INDEX @@@@@@@@
        cmp r2, #3
        blt current_row_0  @ index 0, 1, 2
        cmp r2, #6
        blt current_row_1  @ index 3, 4, 5

@ index 6, 7, 8
current_row_2:
        mov r5, r2         @ register current
        sub r5, r5, #6     @ x_current = i - 6
        mov r6, #2         @ y_current = 2
        b current_done

@ index 3, 4, 5
current_row_1:
        mov r5, r2         @ register current
        sub r5, r5, #3     @ x_current = i - 3
        mov r6, #1         @ y_current = 1
        b current_done

@ index 0, 1, 2
current_row_0:
        mov r5, r2         @ x_current = i
        mov r6, #0         @ y_current = 0

current_done:

        @@@@@@@@ FINDING TARGET INDEX @@@@@@@@
        cmp r4, #3
        blt target_row_0  @ index 0, 1, 2
        cmp r4, #6
        blt target_row_1  @ index 3, 4, 5

@ index 6, 7, 8
target_row_2:
        mov r7, r4        @ register index target
        sub r7, r7, #6    @ x_target = target - 6
        mov r8, #2        @ y_target = 2
        b target_done

@ index 3, 4, 5
target_row_1:
        mov r7, r4        @ register index target
        sub r7, r7, #3    @ x_target = target - 3
        mov r8, #1        @ y_target = 1
        b target_done

@ index 0, 1, 2
target_row_0:
        mov r7, r4        @ x_target = target
        mov r8, #0        @ y_target = 0

target_done:

        @@@@@@@@ COMPUTING DIFFERENCE @@@@@@@@
        subs  r9, r5, r7   @ dx = x_current - x_target
        rsbmi r9, r9, #0   @ if negative, dx = -dx

        subs  r10, r6, r8  @ dy = y_current - y_target
        rsbmi r10, r10, #0 @ if negative, dy = -dy

        add r1, r1, r9     @ add |x - x'|
        add r1, r1, r10    @ add |y - y'|

end_loop_manhattan:
        add r2, r2, #1      @ go forward in the loop
        cmp r2, #9          @ compare with 9
        bne loop_manhattan  @ branch until equal to 9

        mov r0, r1          @ mov sum to return register

        ldmfd sp!, {r4-r12, lr}
        mov   sp, r12
        mov   pc, lr
