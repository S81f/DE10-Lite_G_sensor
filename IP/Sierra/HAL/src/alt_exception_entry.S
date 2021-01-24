/*
  For use in:
                  Sierra Real-Time Kernel
                         for NiosII
File: alt_exception_entry.S

COPYRIGHT (C) 2009 AGSTU AB
All rights reserved. AGSTU's source code is an unpublished work, and the use of a copyright notice does not imply otherwise. This source code contains confidential, trade-secret material of AGSTU AB. Any attempt at or participation in deciphering, decoding, reverse engineering, or in any way altering the source code is strictly prohibited unless the prior written consent of AGSTU AB is obtained.
Disclaimer
All code in this file are provided "as is" and without any warranties expressed or implied, including but not limited to implied warranties of merchantability and fitness for a particular purpose. In no event should the author be liable for any damages whatsoever (including without limitation, damages for loss of business profits, business interruption, loss of business information, or any other pecuniary loss) arising out of the use or inability to use information (including software, designs and files) provided on this site.
  
------------------------------------------------------------------------*/  
 
#include "system.h"
#include "altera_avalon_sierra_name.h"


 /* 
  * !S - Sierra - Nios II sw ver 2
  * 
  * !S - All comments marked with !S indicates deviations from original 
  * component/altera_nios2/HAL/src/alt_exception_entry.S file for specific 
  * changes required by the Sierra kernel. 
  * 
  * This file also requires alt_exception_trap.S with Sierra kernel specific 
  * changes.
  *
  * Trap can only be used by Sierra service calls. The default behaviour of trap 
  * is unimplemented in Nios2.  
  */


/*
 * !S Sierra specifics
 */
#include "altera_avalon_sierra_tcb_offset.h"
#define CPUstatusB_reg 		SIERRA_RTOS_BASE + 0x30        # !S Sierra register

/*
 * This is the exception entry point code, which saves all the caller saved
 * registers and then handles the appropriate exception.  It should be pulled
 * in using a .globl from all the exception handler routines.  This scheme is
 * used so that if an interrupt is never registered, then this code will not
 * appear in the generated executable, thereby improving code footprint.
 */
 
        /*
         * Explicitly allow the use of r1 (the assembler temporary register)
         * within this code. This register is normally reserved for the use of
         * the assembler.
         */
        .set noat

		/*
		 * !S Sierra specifics
		 */
		.globl context_switch
		.globl interrupt_testing
		.globl sierra_trap
		.globl sierra_irq_or_trap
		
        /*
         * The top and bottom of the exception stack
         */
#ifdef ALT_EXCEPTION_STACK

        .globl __alt_exception_stack_pointer

#ifdef ALT_STACK_CHECK

        .globl __alt_exception_stack_limit

        /*
         * We need to store the value of the stack limit after interrupt somewhere.
         */
        .globl  alt_exception_old_stack_limit

#endif
#endif

        .section .exceptions.entry.label, "xa"

        .globl alt_exception
        .type alt_exception, @function
alt_exception:

        .section .exceptions.entry, "xa"

#ifdef ALT_EXCEPTION_STACK

#ifdef ALT_STACK_CHECK
        stw   et, %gprel(alt_exception_old_stack_limit)(gp)
#endif

        movhi et, %hiadj(__alt_exception_stack_pointer - 80)
        addi  et, et, %lo(__alt_exception_stack_pointer - 80) 
        stw   sp, 76(et)
        mov   sp, et

#ifdef ALT_STACK_CHECK
        movhi et, %hiadj(__alt_exception_stack_limit)
        addi  et, et, %lo(__alt_exception_stack_limit) 
        stw   et, %gprel(alt_stack_limit_value)(gp)
#endif

#else
        /* 
         * Process an exception.  For all exceptions we must preserve all
         * caller saved registers on the stack (See the Nios2 ABI
         * documentation for details).
         */
         
        /*
         * !S Is the exception caused by a Sierra HW IRQ?
         */
        addi  	sp, sp, -8			 	# used registers to stack
        stwio  	r2, 0(sp) 
        stwio  	at, 4(sp)
        
        movia 	at, CPUstatusB_reg
   	    ldwio 	at, 0(at)    			# Read Status-register B
   	    andi  	at, at, 1               # Only lowest bit is interesting
   	    bgtu  	at, r0, context_switch 	# If the bit is set (at>0), it is  
   	    						      	# a Sierra HW irq (HW ctx swtich)

        /*
         * !S Is the exception caused by a Sierra trap?
         */
        ldwio r2, -4(ea)              	/* last instr. op-code is put in r2 */ 
        movhi at, 0x003b              	/* upper half of trap opcode */
        ori   at, at, 0x683a          	/* lower half of trap opcode */
        beq   r2, at, sierra_trap 		# Sierra trap (SW ctx switch)
           
        /*
         * !S Not Sierra irq or exception. Restore r2, at and stack
         */   

       ldwio r2, 0(sp)
       ldwio at, 4(sp)
       addi sp, sp, 8 					# sp now in original position (1/3)

# normal exception.entry starts here       
       addi  sp, sp, -76

#ifdef ALT_STACK_CHECK

        bltu  sp, et, .Lstack_overflow

#endif

#endif

        stw   ra,  0(sp)

        /*
         * Leave a gap in the stack frame at 4(sp) for the muldiv handler to
         * store zero into.
         */

        stw   r1,   8(sp)
        stw   r2,  12(sp)
        stw   r3,  16(sp)
        stw   r4,  20(sp)
        stw   r5,  24(sp)
        stw   r6,  28(sp)
        stw   r7,  32(sp)

        rdctl r5, estatus

        stw   r8,  36(sp)
        stw   r9,  40(sp)
        stw   r10, 44(sp)
        stw   r11, 48(sp)
        stw   r12, 52(sp)
        stw   r13, 56(sp)
        stw   r14, 60(sp)
        stw   r15, 64(sp)

        stw   r5,  68(sp) 		/* r5=estatus */ 
        addi  r15, ea, -4 		/* re-issue the interrupted instruction */
        stw   r15,  72(sp)
        
        br 	  interrupt_testing # !S Branch added to avoid sierra routines


	/*
	 * !S Handle first first irq differently 
 	 */
context_switch:

       /* 
        * Do not save context on first tsw interrupt 
        * (no task status needs to be saved) 
        */

   		movia 	r2, first_csw 
   		ldwio   at, 0(r2)
   		bgtu  	at, r0, sierra_irq_or_trap		# Jump to if first_csw != 0
   		
   		addi  	at, r0, 1            			# Set first_csw = 1
   		stwio   at, 0(r2)						# first_csw(et) <- (at)
		
		ldwio	r2, 0(sp)					# restore r2, at and stack
		ldwio 	at, 4(sp) 
   		addi 	sp, sp, 8					# sp now in original position (2/3)
   		rdctl 	r5, estatus
 		br 		interrupt_testing			# results in jump to HW_CTX_SWITCH 
 		    	
	/* 
	 * !S Save content of registers into running tasks tcb 
	 * (hw irq or sw trap) 
	 */
sierra_trap: 

		addi ea, ea, 4		/* !S add ea temporary so save register can be used 
		                       by both hw irq and sw trap */
		
		
sierra_irq_or_trap:

		ldwio 	r2, 0(sp)

		ldwio 	at, 4(sp) 				
    	stwio	at, TCB_REG_AT(at)
       	
       	addi 	sp, sp, 8			  # sp now in original position (3/3)
       	       	
       	movia	at, current_tcb		  # at = current_tcb
	    ldwio   at, 0(at)			  # Get address to current_tcb variable
 
		stwio	r2, TCB_REG_RETURNVALUES+0(at)
   		stwio	r3, TCB_REG_RETURNVALUES+4(at)
    
   		stwio	r4, TCB_REG_ARG+0(at)
   		stwio	r5, TCB_REG_ARG+4(at)
   		stwio	r6, TCB_REG_ARG+8(at)
		stwio 	r7, TCB_REG_ARG+12(at)
	
		rdctl r5, estatus 
	
		stwio	r8,  TCB_REG_GPR+0(at)
		stwio	r9,  TCB_REG_GPR+4(at)
		stwio	r10, TCB_REG_GPR+8(at)
		stwio	r11, TCB_REG_GPR+12(at)
		stwio	r12, TCB_REG_GPR+16(at)
		stwio	r13, TCB_REG_GPR+20(at)
		stwio	r14, TCB_REG_GPR+24(at)
		stwio	r15, TCB_REG_GPR+28(at)
	
 		# r16-r23 is callee saved registers
		/* r24, r25 not affected */
		/* r26, GP not affected */
	
    	stwio	sp, TCB_STACKPTR(at) 	# r27
	
		stwio	r28, TCB_REG_FP(at)

        addi  	r15, ea, -4 	        # s49 nios ref, hw isr must substract -4 from ea
        								# trap case is handled later
        stwio   r15, TCB_REG_EA(at)		# r29 return address from interrupt

		/* r30 not affected */
		stwio	ra, TCB_REG_RA(at)		# r31 return address  to subrutine


interrupt_testing:
        
        /*
         * The interrupt testing code goes here.  If an interrupt is active
         * then it stores ea-4 into 72(sp), handles the interrupt and jumps to
         * .exceptions.exit.  If there is no interrupt then it continues
         */

        .section .exceptions.notirq, "xa"


		/*
		 * !S Sierra trap ends up here
		 */
		# !S If necessary, verify trap instruction here 
        #stw   ea,  72(sp) /* Return after the instruction which caused the exception */
        #ldw   r2,  -4(ea)  /* last instr. op-code is put in r2 for use in .Ltrap_handler (SN) */
		
        /*
         * The other exception handling code goes here.
         */
         
         #/////////////////////////////////
         br .Ltrap_handler   # !S taskswitch trap 
		 #//////////////////////////////////////
		
        .section .exceptions.unknown

        /*
        *  If you get here then one of the following could have happened:
        *
        *  - Your program could have been compiled for a full-featured Nios II
        *    core, but it is running on a smaller core, and instruction emulation
        *    has been disabled by defining ALT_NO_INSTRUCTION_EMULATION.
        *
        *    You can work around the problem by re-enabling instruction emulation,
        *    or you can figure out why your program is being compiled for a system
        *    other than the one that it is running on.
        *
        *  - Your program has executed a trap instruction, but has not implemented
        *    a handler for this instruction.
        *
        *  - Your program has executed an illegal instruction (one which is not
        *    defined in the instruction set).
        *
        *  - Your hardware is broken and is generating spurious interrupts (a
        *    peripheral which deasserts its interrupt output before its interrupt
        *    handler has been executed will cause spurious interrupts).
        *
        */

#ifdef NIOS2_HAS_DEBUG_STUB

        /*
        *  Either tell the user now (if there is a debugger attached) or go into
        *  the debug monitor which will loop until a debugger is attached.
        */
        break

#else

        /*
        *  If there is no debug stub then a BREAK will probably cause a reboot.
        *  An infinate loop will probably be more useful.
        */
0:
        br   0b

#endif

        .section .exceptions.exit, "xa"

        /* 
         * Restore the saved registers, so that all general purpose registers 
         * have been restored to their state at the time the interrupt occured.
         */

        ldw   r5,  68(sp)
        ldw   ea,  72(sp)
        ldw   ra,   0(sp)

        wrctl estatus, r5

        ldw   r1,   8(sp)
        ldw   r2,  12(sp)
        ldw   r3,  16(sp)
        ldw   r4,  20(sp)
        ldw   r5,  24(sp)
        ldw   r6,  28(sp)
        ldw   r7,  32(sp)

#ifdef ALT_EXCEPTION_STACK
#ifdef ALT_STACK_CHECK
        ldw   et, %gprel(alt_exception_old_stack_limit)(gp)
#endif
#endif

        ldw   r8,  36(sp)
        ldw   r9,  40(sp)
        ldw   r10, 44(sp)
        ldw   r11, 48(sp)
        ldw   r12, 52(sp)
        ldw   r13, 56(sp)
        ldw   r14, 60(sp)
        ldw   r15, 64(sp)
        
#ifdef ALT_EXCEPTION_STACK

#ifdef ALT_STACK_CHECK
        stw   et, %gprel(alt_stack_limit_value)(gp)
        stw   zero, %gprel(alt_exception_old_stack_limit)(gp)
#endif

        ldw   sp,  76(sp)

#else
        addi  sp, sp, 76

#endif

        /*
         * Return to the interrupted instruction.
         */

        eret


#ifdef ALT_STACK_CHECK

.Lstack_overflow:
        break 3

#endif

