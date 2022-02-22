/**
 * @file     ctxsw.s
 * @provides ctxsw
 *
 * COSC 3250 / COEN 4820 Assignment 4
 ** COSC 3250 - Project #2
 * This code is helping xinu keep secerts from his babymama
 * @author [JJ Mansour, Steve McDonough]
 * Instructor [Dennis Brylow]
 * TA-BOT:MAILTO [jack.mansour@marquette.edu, stephen.mcdonough@marquette.edu];]
*/
/* Embedded XINU, Copyright (C) 2013.  All rights reserved. */

#include <arm.h>

.text
	.align 4
	.globl	ctxsw


/**
 * @fn void ctxsw(&oldstack, &newstack)
 *
 * Switch context (values in registers) to another process, saving the
 * current processes information. This function will not return as normally
 * thought as it will load in the stack pointer for a different process and
 * jump to that location and begin executing code.
 *
 * @param  &oldstack address of outgoing stack save area
 * @param  &newstack address of incoming stack save area
 * @return special case -- see above
 */
ctxsw:

// TODO: Save process state, including at least the 
//	callee-save ("non-volatile") registers.
	 str r0,[sp, #CTX_R0 *4]
	 str r1,[sp, #CTX_R1 *4]
	 str r2,[sp, #CTX_R2 *4]
	 str r3,[sp, #CTX_R3 *4]
	 str r4,[sp, #CTX_R4 *4]
	 str r5,[sp, #CTX_R5 *4]
	 str r6,[sp, #CTX_R6 *4]
	 str r7,[sp, #CTX_R7 *4]
	 str r8,[sp, #CTX_R8 *4]
	 str r9,[sp, #CTX_R9 *4]
	 str r10,[sp, #CTX_R10 *4]
	 str r11,[sp, #CTX_R11 *4]

	 str r14,[sp, #CTX_SP *4]
	 str r14,[sp, #CTX_LR *4]
	 str r13,[r0]
	 str r13,[r1]
//GET RIDD OF EXTRA R14 AND R13
//SWITCH TO A PLACE WHERE WE ARE STORING 
//STORE WHATSW IN STR, SP[ R0 ]
//LOAD WHAT IS R1 INTO SP 
str sp,[r0]
ldr sp,[r1]



// TODO: Restore process state, including at least the
//	callee-save ("non-volatile") registers.
//	These call and restore segments must match the register data
//	layout you choose in create().
	ldr r0,[sp, #CTX_R0 *4]
	ldr r1,[sp, #CTX_R1 *4]
	ldr r2,[sp, #CTX_R2 *4]
	ldr r3,[sp, #CTX_R3 *4]
	ldr r4,[sp, #CTX_R4 *4]
	ldr r5,[sp, #CTX_R5 *4]
	ldr r6,[sp, #CTX_R6 *4]
	ldr r7,[sp, #CTX_R7 *4]
	ldr r8,[sp, #CTX_R8 *4]
	ldr r9,[sp, #CTX_R9 *4]
	ldr r10,[sp, #CTX_R10 *4]
	ldr r11,[sp, #CTX_R11 *4]

// TODO: Jump to next function.
//	Note: there are two important values at play here --
//	the address of the process's function that we want to end up in
//	once we leave this ctxsw(), and the value we want to have in the
//	link register.  Those are almost always the same value, except
//	for the very first time we ctxsw() to a new process, when they
//	are different.  See create() for more details.
	ldr r14, [sp,#CTX_SP *4]
	ldr r12, [sp, #CTX_LR *4]

// Note: This bx lr line immediately returns from whence we came.  This
//	is here as a placeholder so that you don't crash your O/S when
//	you call ctxsw() before it is complete.  But, this line of code
//	is not actually a part of a completed solution.
	bx	lr

	.end ctxsw
