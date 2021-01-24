#ifndef ___SIERRA_TASK_C__
#define ___SIERRA_TASK_C__

/*!----------------------------------------------------------------------
                  Sierra Real-Time Kernel
 
  Filename      :  sierra_task.c
  
  Company       :  AGSTU AB
  Date Created  :  20150110
  Description   :  This file contains: 
                   - the sierra service calls for task handling,
  Note          :  
  History:
20150110: Added delete task
20160307: add task_change_prio (ver 9.2)

COPYRIGHT (C) AGSTU AB
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
  Task management
-------------------------------------------------------------------------*/

/*!----------------------------------------------------------------------------
  Function    : task_create
  Description : Create a new task and initialize its TCB.
  Arguments   : int taskID             
                int priority           
                int taskstate          state of task after creation
                void (*taskptr)(void)  task function pointer
                void *stackptr         pointer to beginning of task's 
                                       stack area
                int stacksz            size of stack area (in bytes)

  Return      : none
-----------------------------------------------------------------------------*/
void task_create(int taskID,
                   int priority,
                   int taskstate,
                   void (*taskptr)(void),
                   void *stackptr,
                   int stacksz)
{
  tcb_t *newTask;
  svc_t svc;
  statusA_union status;

  newTask = &(TCB_LIST[taskID]);

  /* Parameters to tcb*/
  newTask->taskID = taskID;
  newTask->task = taskptr;
  newTask->ea_reg = taskptr;  
  newTask->ra_reg = taskptr;
  newTask->stacktop = stackptr;
  /* Stack counts backwards, set pointer to highest address */
  newTask->stack = (unsigned int *) (stackptr + stacksz - sizeof(int));
  newTask->stacksz = stacksz;
  newTask->fp_reg = stackptr;
  newTask->priority = priority;

  svc.task_create.type = sierra_task_create;
  svc.task_create.state = taskstate;
  svc.task_create.priority = priority;
  svc.task_create.taskID = taskID;

  /* Start service call*/
  IOWR_ALT_SVC_REGISTER(svc.svc_input);

  //  ack_wait(&status);
  do {
	status.statusA_reg_integer = M_RD_Sierra_statusA_reg;
  } while (status.statusA_t.svc_ack == 1);

}

/*-----------------------------------------------------------------------------
  Function    : task_getinfo
  Description : Retrieves information of a task.
  Arguments   : int taskID

  Return      : Datastructure of type task_info_t.
-----------------------------------------------------------------------------*/
task_info_t task_getinfo(int taskID)
{
  svc_t svc;
  unsigned int info;
  statusA_union status;

  svc.task_getinfo.type =  sierra_task_getinfo;
  svc.task_getinfo.taskID = taskID;
  
  /* Start service call*/
  IOWR_ALT_SVC_REGISTER(svc.svc_input);

  /* return vale */
  //  ack_wait
  do {
	status.statusA_reg_integer = M_RD_Sierra_statusA_reg;
  } while (status.statusA_t.svc_ack == 1);
    
//  info  = status.statusA_t.svc_return & 0x3FF; 
  info  = status.statusA_t.svc_return;

  return *(task_info_t *) &info;
}

/*-----------------------------------------------------------------------------
  Function    : task_start
  Description : Sets state of task to READY.
  Arguments   : int taskID

  Return      : none
-----------------------------------------------------------------------------*/
void task_start(int taskID)
{
  svc_t svc;
  statusA_union status;
  
  svc.task_start.type =  sierra_task_start;
  svc.task_start.taskID = taskID;
  
  /* Start service call*/
  IOWR_ALT_SVC_REGISTER(svc.svc_input);
  
  do {
	status.statusA_reg_integer = M_RD_Sierra_statusA_reg;
  } while (status.statusA_t.svc_ack == 1);
  
}

/*-----------------------------------------------------------------------------
  Function    : task_block
  Description : Block the runing task
  Arguments   : none
  Return      : none
-----------------------------------------------------------------------------*/

void task_block(int taskID)
{
  svc_t svc;
  statusA_union statusA;
  int retval;

  svc.task_block.type =  sierra_task_block;
  svc.task_block.taskID = taskID;

  /* Start service call*/
  IOWR_ALT_SVC_REGISTER(svc.svc_input);

  //  ack_wait
  do {
    statusA.statusA_reg_integer = M_RD_Sierra_statusA_reg;
  } while (statusA.statusA_t.svc_ack == 1);
 
  retval=statusA.statusA_t.svc_return & 0x3f;

//if runing task_id blocked the return is true (1) else false (0)
  if (0 != (retval & 0x1)) { /*Task becomes blocked*/
    /* Get next task ID */
    NEXT_TASKID = constant_task_mask & (retval >> 1);
    taskswitch; /* perform manual contextswitch */
  }

// NEXT_TASKID = constant_task_mask & statusA.statusA_t.svc_return;
 
//  taskswitch; /* perform manual contextswitch */

}

/*-----------------------------------------------------------------------------
  Function    : Delete runing task
  Description : Move it to dormat and it has to be created againe
  Arguments   : none
  Return      : none
-----------------------------------------------------------------------------*/


void task_delete(void)
{
  svc_t svc;
  statusA_union statusA;

  svc.wait_for_next_period.type =  sierra_task_delete;

  /* Start service call*/
  IOWR_ALT_SVC_REGISTER(svc.svc_input);

  //  ack_wait
  do {
    statusA.statusA_reg_integer = M_RD_Sierra_statusA_reg;
  } while (statusA.statusA_t.svc_ack == 1);

  /* Get next task ID */

 NEXT_TASKID = constant_task_mask & statusA.statusA_t.svc_return;

  taskswitch; /* perform manual contextswitch */

}

/*-----------------------------------------------------------------------------
  Function    : task_change_prio
  Description : Sets state of task to READY.
  Arguments   : int taskID and int priority

  Return      : none
-----------------------------------------------------------------------------*/
void task_change_prio(int taskID, int priority)
{
  svc_t svc;
  statusA_union status;
  
  svc.task_start.type =  sierra_task_change_prio;
  svc.task_create.priority = priority;
  svc.task_start.taskID = taskID;
  
  /* Start service call*/
  IOWR_ALT_SVC_REGISTER(svc.svc_input);
  
  do {
	status.statusA_reg_integer = M_RD_Sierra_statusA_reg;
  } while (status.statusA_t.svc_ack == 1);
  
}

/*-----------------------------------------------------------------------------
  Function    : task_yield_EJ_Testad !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  Description : Yields to a task of same priority, if one exists.
  Arguments   : none

  Return      : none
-----------------------------------------------------------------------------*/
void task_yield(void)
{
  statusA_union statusA;
  statusB_union statusB;
  svc_t svc;
 
  svc.task_yield.type = sierra_task_yield;

  /* Start service call*/
  IOWR_ALT_SVC_REGISTER(svc.svc_input);

  do {
	statusA.statusA_reg_integer = M_RD_Sierra_statusA_reg;
  } while (statusA.statusA_t.svc_ack == 1);

  
statusB.statusB_reg_integer = M_RD_Sierra_statusB_reg;

  /* Get next task ID */
NEXT_TASKID = constant_task_mask & statusB.statusB_t.running_taskID;

  taskswitch; /* perform manual contextswitch */
}

#endif // __SIERRA_TASK_C__
