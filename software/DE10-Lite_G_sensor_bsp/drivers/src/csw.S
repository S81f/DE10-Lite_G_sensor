/*!----------------------------------------------------------------------
                  Sierra Real-Time Kernel
                         for NiosII
 
  Filename      :  csw.S
  
  Company       :  AGSTU AB
  Date Created  :  2006-08
  Description   :  This file contains: 
                :  Context switch routine for Sierra.
                :  hw_irq
                :  sw trap (manual taskswitch)
  Note          :  
 
COPYRIGHT (C) 2009 AGSTU AB
All rights reserved. AGSTU's source code is an unpublished work, and the use of a copyright notice does not imply otherwise. This source code contains confidential, trade-secret material of AGSTU AB. Any attempt at or participation in deciphering, decoding, reverse engineering, or in any way altering the source code is strictly prohibited unless the prior written consent of AGSTU AB is obtained.
Disclaimer
All code in this file are provided "as is" and without any warranties expressed or implied, including but not limited to implied warranties of merchantability and fitness for a particular purpose. In no event should the author be liable for any damages whatsoever (including without limitation, damages for loss of business profits, business interruption, loss of business information, or any other pecuniary loss) arising out of the use or inability to use information (including software, designs and files) provided on this site.


 * Sierra - Nios II 
 */

#include "altera_avalon_sierra_tcb_offset.h"

	/*
     * Explicitly allow the use of r1 (the assembler temporary register)
     * within this code. This register is normally reserved for the use of
     * the compiler.
     */
	.set noat
	
	.data
	
	.text
	
	.global HW_CTX_SWITCH
	.global SW_CTX_SWITCH
	.global gettask 
	
	/**********************************************
	  context switch routine
	**********************************************/

/* registers have been saved into running tcb in exception entry handler */
HW_CTX_SWITCH:
	br gettask

gettask:
   	call get_next_task //call c-function and return here
	br restore_context
	
restore_context:
	
	movia	at, current_tcb			# at=current_tcb
	ldwio   at, 0(at)				# Get address to current_tcb variable
 
   	/* re-store r1 (at) last */
	ldwio	r2, TCB_REG_RETURNVALUES+0(at)	
   	ldwio	r3, TCB_REG_RETURNVALUES+4(at)
    
   	ldwio	r4, TCB_REG_ARG+0(at)
   	ldwio	r5, TCB_REG_ARG+4(at)
   	   	
   	ldwio	r6, TCB_REG_ARG+8(at)
	ldwio 	r7, TCB_REG_ARG+12(at)
	
	ldwio	r8, TCB_REG_GPR+0(at)
	ldwio	r9, TCB_REG_GPR+4(at)
	ldwio	r10, TCB_REG_GPR+8(at)
	ldwio	r11, TCB_REG_GPR+12(at)
	ldwio	r12, TCB_REG_GPR+16(at)
	ldwio	r13, TCB_REG_GPR+20(at)
	ldwio	r14, TCB_REG_GPR+24(at)
	ldwio	r15, TCB_REG_GPR+28(at)
	#ldwio	r16, TCB_REG_GPR+32(at)
	#ldwio	r17, TCB_REG_GPR+36(at)
	#ldwio	r18, TCB_REG_GPR+40(at)
	#ldwio	r19, TCB_REG_GPR+44(at)
	#ldwio	r20, TCB_REG_GPR+48(at)
	#ldwio	r21, TCB_REG_GPR+52(at)
	#ldwio	r22, TCB_REG_GPR+56(at)
	#ldwio	r23, TCB_REG_GPR+60(at)
	/* r24, r25, r26 not affected */

	ldwio	sp, TCB_STACKPTR(at) # r27 stack pointer 
	ldwio	fp, TCB_REG_FP(at)	 # r28 fram pointer
	ldwio	ea, TCB_REG_EA(at)   # r29 return address from interrupt
	
	/* r30 not affected */
	
	ldwio	ra, TCB_REG_RA(at) # r31
	ldwio	at, TCB_REG_AT(at)
		
   	/* Jump to new task, adr in ea */
	eret 	# return from exception
			# eret: status <- estatus, PC <- ea
			# status <- 0x1 (allow interrupts agian)
			# both retrun from hw_ctx and trap (sw_ctx)
	


/* registers have been saved into running tcb in exception entry handler */	
SW_CTX_SWITCH:
	/*** Get new task id ***/
	call get_new_task
	
	br restore_context
/////////////////////////////////////////////	


   	

