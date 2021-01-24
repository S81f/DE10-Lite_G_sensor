#ifndef ___SIERRA_SEMFLAG_C__
#define ___SIERRA_SEMFLAG_C__

/*!----------------------------------------------------------------------
                  Sierra Real-Time Kernel
 
  Filename      :  sierra_sem.c (also flag handler)
  
  Company       :  AGSTU AB
  Date Created  :  2006
  Description   :  This file contains: 
                   - the sierra service calls for time handling,
  Note          :  
 
COPYRIGHT (C) 2009, 2010 - AGSTU AB
All rights reserved. AGSTU's source code is an unpublished work, and the use of a copyright notice does not imply otherwise. This source code contains confidential, trade-secret material of AGSTU AB. Any attempt at or participation in deciphering, decoding, reverse engineering, or in any way altering the source code is strictly prohibited unless the prior written consent of AGSTU AB is obtained.
Disclaimer
All code in this file are provided "as is" and without any warranties expressed or implied, including but not limited to implied warranties of merchantability and fitness for a particular purpose. In no event should the author be liable for any damages whatsoever (including without limitation, damages for loss of business profits, business interruption, loss of business information, or any other pecuniary loss) arising out of the use or inability to use information (including software, designs and files) provided on this site.
    
-----------------------------------------------------------------------------*/
#include <altera_avalon_sierra_io.h>
#include <altera_avalon_sierra_regs.h>
#include "altera_avalon_sierra_ker.h"
#include <altera_avalon_sierra_name.h>
#include <altera_avalon_sierra_io.h>
#include <altera_avalon_sierra_tcb.h>
#include <stdio.h>
#include <system.h>
#include <io.h>

/*!-----------------------------------------------------------------------
  Semaphore management
-------------------------------------------------------------------------*/

/*!----------------------------------------------------------------------------
  Function    : sem_take
  Description : Take a semaphore.
  Arguments   : int semid - Semaphore ID number

  Return      : none
-----------------------------------------------------------------------------*/
void sem_take(int semid)
{
  svc_t svc;
  statusA_union status;
  int retval;
  
  svc.take_sem.type =  sierra_sem_take;
  svc.take_sem.semid =  semid;

#ifdef DEF_LOGGING_PRINT  
printf("\nSVC;Sem_take nr: = %d\n", semid); 
#endif


  /* Start service call*/
  IOWR_ALT_SVC_REGISTER(svc.svc_input);

  //  ack_wait
  do {
	status.statusA_reg_integer = M_RD_Sierra_statusA_reg;
  } while (status.statusA_t.svc_ack == 1);

  retval=status.statusA_t.svc_return & 0x3f;

  if (0 != (retval & 0x1)) { /*Task becomes blocked*/
    NEXT_TASKID = constant_task_mask & (retval >> 1);
    taskswitch; /* perform manual contextswitch */
  }

}
/*!----------------------------------------------------------------------------
  Function    : sem_release
  Description : Release a semaphore.
  Arguments   : int semid - Semaphore ID number

  Return      : none
-----------------------------------------------------------------------------*/
void sem_release(int semid)
{
  svc_t svc;
  statusA_union statusA;

  svc.release_sem.type =  sierra_sem_release;
  svc.release_sem.semid =  semid;

#ifdef DEF_LOGGING_PRINT  
printf("\nSVC;Sem_realease nr: = %d\n", semid); 
#endif


  /* Start service call*/
  IOWR_ALT_SVC_REGISTER(svc.svc_input);

  //  ack_wait
  do {
	statusA.statusA_reg_integer = M_RD_Sierra_statusA_reg;
  } while (statusA.statusA_t.svc_ack == 1);
}

/*!----------------------------------------------------------------------------
  Function    : sem_read
  Description : Read the value of semaphore.
  Arguments   : int semid - Semaphore ID number

  Return      : Datastructure of type sem_info_t. 
-----------------------------------------------------------------------------*/
sem_info_t sem_read(int taskID)
{
  svc_t svc;
  statusA_union status;
  unsigned char info;
  sem_info_t info_t;
  
  svc.read_sem.type =  sierra_sem_read;
  svc.read_sem.taskID =  taskID;

  /* Start service call*/
  IOWR_ALT_SVC_REGISTER(svc.svc_input);

  //  ack_wait
  do {
	status.statusA_reg_integer = M_RD_Sierra_statusA_reg;
  } while (status.statusA_t.svc_ack == 1);

  info = status.statusA_t.svc_return & 0x7f;
  return *(sem_info_t *)&info;
}

/*!-----------------------------------------------------------------------
  Flags
-------------------------------------------------------------------------*/
/*!---------------------------------------------------------------------------
  Function    : flag_wait
  Description : Wait for one flagbit, or a combination of flagbits to
                be set.
  Arguments   : int flag_mask - flag bits

  Return      : none
-----------------------------------------------------------------------------*/
void flag_wait(int flag_mask)
{
  svc_t svc;
  statusA_union status;
  int retval;

  svc.flag_wait.type = sierra_flag_wait;
  svc.flag_wait.flag_mask =  flag_mask;

  /* Start service call*/
  IOWR_ALT_SVC_REGISTER(svc.svc_input);

  //  ack_wait
  do {
	status.statusA_reg_integer = M_RD_Sierra_statusA_reg;
  } while (status.statusA_t.svc_ack == 1);

  retval=status.statusA_t.svc_return & 0x3f;

  if (0 != (retval & 0x1)) { /*Task becomes blocked*/
    NEXT_TASKID = constant_task_mask & (retval >> 1);
    taskswitch; /* perform manual contextswitch */
  }
}
/*!---------------------------------------------------------------------------
  Function    : flag_set
  Description : Set one flagbit, or a combination of flagbits.
  Arguments   : int flag_mask - flagbits

  Return      : none
-----------------------------------------------------------------------------*/
void flag_set(int flag_mask)
{
  svc_t svc;
  statusA_union status;

  svc.flag_set.type = sierra_flag_set;
  svc.flag_set.flag_mask =  flag_mask;

  /* Start service call*/
  IOWR_ALT_SVC_REGISTER(svc.svc_input);

  //  ack_wait
  do {
	status.statusA_reg_integer = M_RD_Sierra_statusA_reg;
  } while (status.statusA_t.svc_ack == 1);
}

/*!---------------------------------------------------------------------------
  Function    : flag_clear
  Description : Clear one or several flagbits.
  Arguments   : int flag_mask - flagbits

  Return      : none
-----------------------------------------------------------------------------*/
void flag_clear(int flag_mask)
{
  svc_t svc;
  statusA_union status;

  svc.flag_clear.type = sierra_flag_clear;
  svc.flag_clear.flag_mask =  flag_mask;

  /* Start service call*/
  IOWR_ALT_SVC_REGISTER(svc.svc_input);

  //  ack_wait
  do {
	status.statusA_reg_integer = M_RD_Sierra_statusA_reg;
  } while (status.statusA_t.svc_ack == 1);
}

#endif // __SIERRA_SEMFLAG_C__

