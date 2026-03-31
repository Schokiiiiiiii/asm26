@ Author : Prénom Nom

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
        

	@@@@@@@@@@@ ----------- @@@@@@@@@@

        ldmfd sp!, {r4-r12, lr}
        mov   sp, r12
        mov   pc, lr
