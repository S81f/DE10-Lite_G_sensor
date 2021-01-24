

#ifndef __ALTERA_AVALON_SIERRA_TCB_H__
#define __ALTERA_AVALON_SIERRA_TCB_H__

/*!----------------------------------------------------------------------
                  Sierra Real-Time Kernel
                         for NiosII
 
  Filename      :  sierra_avalon_sierra_tcb.h
  
  Company       :  AGSTU AB
  Date Created  :  2006-08
  Description   :  Task Control Block (TCB) declaration for C use.
  Note          :  
 
COPYRIGHT (C) 2009 AGSTU AB
All rights reserved. AGSTU's source code is an unpublished work, and the use of a copyright notice does not imply otherwise. This source code contains confidential, trade-secret material of AGSTU AB. Any attempt at or participation in deciphering, decoding, reverse engineering, or in any way altering the source code is strictly prohibited unless the prior written consent of AGSTU AB is obtained.
Disclaimer
All code in this file are provided "as is" and without any warranties expressed or implied, including but not limited to implied warranties of merchantability and fitness for a particular purpose. In no event should the author be liable for any damages whatsoever (including without limitation, damages for loss of business profits, business interruption, loss of business information, or any other pecuniary loss) arising out of the use or inability to use information (including software, designs and files) provided on this site.

 * TCB for NiosII 
 */
struct tcb
{
  unsigned int taskID;              // 0
  void (*task);                     // 4
  unsigned int at_reg;              // 8    r1 ass temporary
  unsigned int returnvalue_reg[2];  // 12   r2-r3
  unsigned int arg_reg[4];          // 20   r4-r7
  unsigned int gpr_reg[16];         // 36   r8-r23
  unsigned int et_reg;              // 100  r24 exc temporary 
  unsigned int gp_reg;              // 104  r26
  unsigned int fp_reg;              // 108  r28 frame ptr
  unsigned int ea_reg;              // 112  r29 exc ret adr
  unsigned int ra_reg;              // 116  r31 ret adr                   

  unsigned int *stack;              // 120
  unsigned int *stacktop;           // 132
  unsigned int stacksz;             // 128
  unsigned int collision;           // 
  unsigned int priority;            // 
};

typedef struct tcb tcb_t;

#endif /* __ALTERA_AVALON_SIERRA_TCB_H__ */


