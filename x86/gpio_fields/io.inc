@********************************************************************************
@ Nom du fichier: io.inc
@ Contenu		: Macros pour l'utilisation de la console en mode simulation
@ 
@ Auteur(s)		: Daniel Rossier
@ Ecrit le		: Juin 2008
@ Modifie le	: 
@ Modifications	: 
@
@********************************************************************************

@--------------------------------------------------------------------------------
@ But       : Ecrire un caractere sur la console 
@ Parametres: cond = condition d'execution. 
@             char = registre contenant le caractere a afficher.               
@ Modifie   : r0, r1
@--------------------------------------------------------------------------------
  

.macro PUT_C char
@ save r0-r5
	stmfd 	sp!, {r0-r10, lr}
	mov		r0, \char
	bl 	1f
	ldmfd	sp!, {r0-r10, lr}
	add	pc, pc, #4
1:	
	ldr ip, [r8, #88]
	ldr pc, [ip, #12]
.endm

@ r11 is used to store the character
.macro READ_C char 
	stmfd 	sp!, {r0-r10, lr}
	bl 	1f
	mov	r11, r0
	ldmfd	sp!, {r0-r10, lr}
	mov	\char, r11
	add	pc, pc, #4
1:	
	ldr ip, [r8, #88]
	ldr pc, [ip, #4]
.endm

@ WRITE_S allows the user to write a string to the console.
@ The string is passed via the \str register argument.
.macro WRITE_S	str
	stmfd sp!, {r0, r1}
	mov		r1, \str
5:
	ldrb    r0, [r1], #1     
    cmp     r0, #0		
    beq	5f    
	
	PUT_C	r0
	
	cmp		r0, #13
    bne 5b
    
    mov		r0, #10
    PUT_C	r0
    	
	b 5b	
               
5:
	ldmfd sp!, {r0, r1}
.endm

@ READ_S allows the user to enter a string in the console.
@ The string is then stored in \str

.macro READ_S str
	stmfd sp!, {r0, r1}
	mov		r0, \str
	
5:	READ_C	r1
	PUT_C	r1
	strb	r1, [r0], #1	
	
	cmp		r1, #13
	bne		5b
	
	mov		r1, #10
	PUT_C	r1
	
	mov		r1, #0
	strb	r1, [r0]
	ldmfd sp!, {r0, r1}

.endm



