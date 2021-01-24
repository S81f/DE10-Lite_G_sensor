#ifndef ___SIERRA_TIME_C__
#define ___SIERRA_TIME_C__

/*!----------------------------------------------------------------------
                  Sierra Real-Time Kernel
 
  Filename      :  sierra_time.c
  
  Company       :  AGSTU AB
  Date Created  :  2006
  Description   :  This file contains: 
                   - the sierra service calls for time handling,
  Note          :  
 
COPYRIGHT (C) 2009- AGSTU AB
All rights reserved. AGSTU's source code is an unpublished work, and the use of a copyright notice does not imply otherwise. This source code contains confidential, trade-secret material of AGSTU AB. Any attempt at or participation in deciphering, decoding, reverse engineering, or in any way altering the source code is strictly prohibited unless the prior written consent of AGSTU AB is obtained.
Disclaimer
All code in this file are provided "as is" and without any warranties expressed or implied, including but not limited to implied warranties of merchantability and fitness for a particular purpose. In no event should the author be liable for any damages whatsoever (including without limitation, damages for loss of business profits, business interruption, loss of business information, or any other pecuniary loss) arising out of the use or inability to use information (including software, designs and files) provided on this site.
    
-----------------------------------------------------------------------------*/
#include <altera_avalon_sierra_io.h>
#include <altera_avalon_sierra_regs.h>
#include "altera_avalon_sierra_ker.h"
#include <altera_avalon_sierra_name.h>

#include <altera_avalon_sierra_tcb.h>
#include <stdio.h>
#include <system.h>
#include <io.h>

// Include timer task for logging print with time
#ifdef DEF_LOGGING_PRINT
  #include <altera_avalon_timer_regs.h>
#endif

/*!-----------------------------------------------------------------------
  Time management
-------------------------------------------------------------------------*/
/*!----------------------------------------------------------------------------
  Function    : init_period_time
  Description : Inititialize period time for current task.
  Arguments   : Periodtime (in ticks)

  Return      : none
-----------------------------------------------------------------------------*/
void init_period_time(int per_time)
{
  svc_t svc;
  statusA_union status;
  svc.init_period_time.type =  sierra_init_period_time;
  svc.init_period_time.period =  per_time;

  /* Start service call*/
  IOWR_ALT_SVC_REGISTER(svc.svc_input);

  //  ack_wait
  do {
	status.statusA_reg_integer = M_RD_Sierra_statusA_reg;
  } while (status.statusA_t.svc_ack == 1);
}

/*!----------------------------------------------------------------------------
  Function    : wait_for_next_period
  Description : Let current task wait for next period.
  Arguments   : none
  Return      : none
-----------------------------------------------------------------------------*/
task_periodic_start_union wait_for_next_period(void)
{
  svc_t svc;
//  unsigned int info;
  task_periodic_start_union info;
  unsigned int temp;
  statusA_union statusA;

//#ifdef DEF_LOGGING_PRINT  
//printf("\nSVC; Task ready and wait for next period \n"); 
//#endif

  svc.wait_for_next_period.type =  sierra_wait_for_next_period;


  tsw_off();

  /* Start service call*/
  IOWR_ALT_SVC_REGISTER(svc.svc_input);

  //  ack_wait
  do {
	statusA.statusA_reg_integer = M_RD_Sierra_statusA_reg;
  } while (statusA.statusA_t.svc_ack == 1);
 
  /* Get next task ID */
  info.periodic_start_integer = statusA.statusA_t.svc_return;

NEXT_TASKID = constant_task_mask & info.task_periodic_start_t.task_id;
 
 temp = info.periodic_start_integer;
  tsw_on();
  taskswitch; /* perform manual contextswitch */

  return *(task_periodic_start_union *) & temp; //if you do not use deadline control, you can deleate this row.
}

/*!----------------------------------------------------------------------------
  Function    : delay
  Description : Delay current task.
  Arguments   : Number of ticks to delay task.

  Return      : none
-----------------------------------------------------------------------------*/
void delay(int delay_time)
{
  svc_t svc;
  statusA_union statusA;
 
  svc.delay.type =  sierra_delay;
  svc.delay.nroftick =  delay_time;

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

#endif // __SIERRA_TIME_C__
