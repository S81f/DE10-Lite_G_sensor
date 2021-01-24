/*  For use in:
                  Sierra Real-Time Kernel
                         for NiosII
File: alt_exception_trap.s, This is the trap exception handler for Nios2.
                                                                          
COPYRIGHT (C) 2009 AGSTU AB
All rights reserved. AGSTU's source code is an unpublished work, and the use of a copyright notice does not imply otherwise. This source code contains confidential, trade-secret material of AGSTU AB. Any attempt at or participation in deciphering, decoding, reverse engineering, or in any way altering the source code is strictly prohibited unless the prior written consent of AGSTU AB is obtained.
Disclaimer
All code in this file are provided "as is" and without any warranties expressed or implied, including but not limited to implied warranties of merchantability and fitness for a particular purpose. In no event should the author be liable for any damages whatsoever (including without limitation, damages for loss of business profits, business interruption, loss of business information, or any other pecuniary loss) arising out of the use or inability to use information (including software, designs and files) provided on this site.


------------------------------------------------------------------------*/  

  		 /* !S - Sierra - Nios II
  		 */ 

        .section .exceptions.start
        .globl alt_exception_trap
alt_exception_trap:

        /*
         * Pull in the entry/exit code.
         */
        .globl alt_exception

		#////////////////////////////////// !S
 		.globl .Ltrap_handler 
 		#///////////////////////////////////

        .section .exceptions.soft, "xa"

.Ltrap_handler:

        /*
         * Did a trap instruction cause the exception?
         *
         * The instruction which the exception occurred on has been loaded
         * into r2 by code in alt_exception_entry.S
         *
         */

        #movhi r3,0x003b              /* upper half of trap opcode */
        #ori   r3,r3,0x683a           /* lower half of trap opcode */
        #bne   r2,r3,.Lnot_trap
		# add this for check if not trap
        /*
         *  There is no trap handler defined here, and so executing a trap
         *  instruction causes a software break.  If you provide a trap handler,
         *  then you must replace the break instruction below with your handler.
         *  Your handler must preserve ea and the usual callee saved registers.
         */

        #break    #(SN)

	
		jmpi SW_CTX_SWITCH 

	/*	br SW_CTX_SWITCH  Denna kan ge problem om hopadressen är för lång */

        #//br   .Lexception_exit
        #////////////////////////////////////////////////(SN)

.Lnot_trap:


        .section .exceptions.exit.label
.Lexception_exit:


