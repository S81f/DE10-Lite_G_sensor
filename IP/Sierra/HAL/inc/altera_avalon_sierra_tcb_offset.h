/*!----------------------------------------------------------------------
                  Sierra Real-Time Kernel
                         for NiosII
 
  Filename      :  sierra_avalon_sierra_tcb_offset.h
  
  Company       :  AGSTU AB
  Date Created  :  2006-08
  Description   :  Task Control Block (TCB) declaration for assembler use.
  Note          :  
 
COPYRIGHT (C) 2009 AGSTU AB
All rights reserved. AGSTU's source code is an unpublished work, and the use of a copyright notice does not imply otherwise. This source code contains confidential, trade-secret material of AGSTU AB. Any attempt at or participation in deciphering, decoding, reverse engineering, or in any way altering the source code is strictly prohibited unless the prior written consent of AGSTU AB is obtained.
Disclaimer
All code in this file are provided "as is" and without any warranties expressed or implied, including but not limited to implied warranties of merchantability and fitness for a particular purpose. In no event should the author be liable for any damages whatsoever (including without limitation, damages for loss of business profits, business interruption, loss of business information, or any other pecuniary loss) arising out of the use or inability to use information (including software, designs and files) provided on this site.

 * TCB DECLARATION
 * Describes the offset to each member of the tcb_t,
 * defined in tcb.h
 */

/*
 * NiosII registers (32 bits wide)
 *
 * R0      : Always zero (not saved)
 * R1      : Assemlber Temporary (at)
 * R2      : Return value (least significant 32 bits)
 * R3      : Return value (most significant 32 bits)
 * R4-R7   : Register arguments
 * R8-R15  : Caller-saved general purpose registers
 * R16-R23 : Callee-saved general purpose registers
 * R24     : Exception Temporary (et)
 * R25     : Break Temporary (not saved)
 * R26     : Global pointer (gb)
 * R27     : Stack pointer (sp)
 * R28     : Frame pointer (fp)
 * R29     : Exception Return Address (ea)
 * R30     : Break Return Address (not saved)
 * R31     : Return Address (ra)
 */

/* TCB for NiosII */
#define TCB_TASK_ID            0
#define TCB_STARTADR           4     
#define TCB_REG_AT             8
#define TCB_REG_RETURNVALUES  12  /* 2 registers */
#define TCB_REG_ARG           20  /* 4 registers */
#define TCB_REG_GPR           36  /* 16 registers */
#define TCB_REG_ET           100
#define TCB_REG_GP           104
#define TCB_REG_FP           108
#define TCB_REG_EA           112
#define TCB_REG_RA           116
#define TCB_STACKPTR         120
#define TCB_STACKAREA        124
#define TCB_STACKSIZE        128
  


