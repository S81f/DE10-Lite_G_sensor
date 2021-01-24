/*!----------------------------------------------------------------------
                  Sierra Real-Time Kernel
                         for NiosII
 
  Filename      :  sierra.c
  
  Company       :  AGSTU AB
  Date Created  :  2006-14
  Description   :  This file contains: 
                   - the sierra service calls,
                   - sierra registers
                   - sierra context data 
  Note          :  


COPYRIGHT (C) 2006 - 2014 AGSTU AB
All rights reserved. AGSTU's source code is an unpublished work, and the use of a copyright notice does not imply otherwise. This source code contains confidential, trade-secret material of AGSTU AB. Any attempt at or participation in deciphering, decoding, reverse engineering, or in any way altering the source code is strictly prohibited unless the prior written consent of AGSTU AB is obtained.
Disclaimer
All code in this file are provided "as is" and without any warranties expressed or implied, including but not limited to implied warranties of merchantability and fitness for a particular purpose. In no event should the author be liable for any damages whatsoever (including without limitation, damages for loss of business profits, business interruption, loss of business information, or any other pecuniary loss) arising out of the use or inability to use information (including software, designs and files) provided on this site.
    
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <system.h>
#include <io.h>

/*!----------------------------------------------------------------------------
    Include Sierra driver files
-----------------------------------------------------------------------------*/
#include <altera_avalon_sierra_ker.h>
#include <altera_avalon_sierra_tcb.h>
#include <altera_avalon_sierra_regs.h>
#include <altera_avalon_sierra_io.h>
#include <altera_avalon_sierra_name.h>

// Include timer task for logging print with time
#ifdef DEF_LOGGING_PRINT
  #include <altera_avalon_timer_regs.h>
#endif



/* 
 * Global Sierra variables, to get a good start! ;-)
 */
volatile unsigned int first_csw=0;


/*
 *  Register Sierra HW IRQ with NiosII ISR (HW_CTX_SWTICH in csw.S)
 */
/* #ifdef SIERRA_RTOS_BASE */
static void init_Sierra_interrupt()
{
 // int ret=0xffffffff;
  if (alt_irq_register( SIERRA_RTOS_IRQ, NULL, HW_CTX_SWITCH )) printf("error registration Sierra irq");
 
}
/* #endif */

/* 
 * TCB list 
 */
tcb_t TCB_LIST[N_TASKS];

/* 
 * Context data for currently executing task 
 */
volatile unsigned char RUNNING_TASKID;
volatile unsigned char NEXT_TASKID;
volatile tcb_t *current_tcb = 0;

/*!----------------------------------------------------------------------------
  Function    : Sierra_Version(void)
  Description : Sierra hw version.
  Arguments   : none

  Return      : visions number
-----------------------------------------------------------------------------*/
unsigned int sierra_SW_driver_version(void)
{
  unsigned int svar = 0;
  svar = C_Sierra_SW_driver_version;
return svar;
  }
  
/*!----------------------------------------------------------------------------
  Function    : Sierra_Version(void)
  Description : Sierra hw version.
  Arguments   : none

  Return      : visions number
-----------------------------------------------------------------------------*/
unsigned int sierra_HW_version(void)
{
  unsigned int svar = 0;
  svar = M_Sierra_HW_Version_reg;
return svar;
  }

/*!----------------------------------------------------------------------------
  Function    : print_SierraTime_base_reg
  Description : Prints the value of the Sierra Time_base_register.
  Arguments   : none

  Return      : none
-----------------------------------------------------------------------------*/
unsigned int SierraTime_base_reg(void)
{
  unsigned int svar = 0;
  svar = M_RD_SierraTime_base_reg;
return svar;
}

/*!----------------------------------------------------------------------------
  Function    : set_timebase
  Description : Sets the internal clock-tick timebase for the Sierra.
  Arguments   : int timebase (in hex)

  Return      : none
  
  Note        : Example of how to calculate timebase value based on wanted 
              : tick-time:
              : This example     : 100MHz system-clock
              : Wanted tick time : 10us
              : Formula gives    : 10us x 100MHz / 1000 => 1(dec) or 1(hex)
              : --> set_timebase(0x1);
-----------------------------------------------------------------------------*/
void set_timebase (unsigned int hex)
{
M_IOWR_SierraTime_base_reg(hex);
}

/*!----------------------------------------------------------------------------
  Function    : Sierra_Initiation_HW_and_SW
  Description : Initializes all TCB's and CPUctrl_reg, also reset sierra HW.
  Arguments   : none
  Return      : none

-----------------------------------------------------------------------------*/
void Sierra_Initiation_HW_and_SW(void)
{
  int i;
  //SW reset of HW Sierran
  M_WD_SW_RESET_REGISTER(0x15);
  M_WD_SW_RESET_REGISTER(0x00);
  
 /*Initialize TCBs*/
  for (i = 0; i < N_TASKS; TCB_LIST[i++].taskID = 0xff);
  current_tcb=NULL;
  
  /* Initiate Sierra hw irq for NiosII*/
    init_Sierra_interrupt();

#ifdef DEF_LOGGING_PRINT
printf("\n Logging print function is activ and alocate/start the timer\n");
TIMER_RESET();
TIMER_START();
#else
printf("\n Logging print function NOT activ and timer free\n");
#endif

}

/*!----------------------------------------------------------------------------
  Function    : get_next_task
  Description : Retrieves context data on next task (used when
                the Sierra interrupts for contextswitch).
  Arguments   : none
 
  Return      : none
-----------------------------------------------------------------------------*/
void get_next_task(void)
{
  statusB_union status;
  ctrl_union CPU_CTRL_reg;

  /* Ack. on TSW req */
  CPU_CTRL_reg.ctrl_t.tswoff_req = 0;
  CPU_CTRL_reg.ctrl_t.tsw_req_ack = 1;
  IOWR_ALT_CTRL_REGISTER(CPU_CTRL_reg.ctrl_integer);
  
  do {
    //MB: status = CPUstatusB_reg;
  //  status = *CPUstatusB_reg;
      status.statusB_reg_integer = M_RD_Sierra_statusB_reg;
  } while (status.statusB_t.tsw_req_flag != 0);

  /* Get next task ID */
RUNNING_TASKID = constant_task_mask & status.statusB_t.running_taskID;

#ifdef DEF_LOGGING_PRINT  
printf("\nTSW; Task preemted, new taskid = %d, Time = %d \n", RUNNING_TASKID, ((TIMER_READ() & 0xFFFFE000)>>13));
//printf("TSW; Task preemted, new taskid = %d, Time = %d\n", RUNNING_TASKID, TIMER_READ()); 

/* logging */
#endif
  
  current_tcb = &TCB_LIST[RUNNING_TASKID];
 
  /* Nack on TSW req */
  CPU_CTRL_reg.ctrl_t.tsw_req_ack = 0;
  IOWR_ALT_CTRL_REGISTER(CPU_CTRL_reg.ctrl_integer);
}

/*!----------------------------------------------------------------------------
  Function    : tsw_on
  Description : Enables scheduling and all interrupts.
  Arguments   : none

  Return      : none
-----------------------------------------------------------------------------*/
void tsw_on(void)
{
  /* Current CPU control status */
  ctrl_union CPU_CTRL_reg;
 
  CPU_CTRL_reg.ctrl_t.tswoff_req = 0;
  CPU_CTRL_reg.ctrl_t.tsw_req_ack = 0;

IOWR_ALT_CTRL_REGISTER(CPU_CTRL_reg.ctrl_integer);
}

/*!----------------------------------------------------------------------------
  Function    : tsw_off
  Description : Disables scheduling and all interrupts.
  Arguments   : none

  Return      : none
-----------------------------------------------------------------------------*/
void tsw_off(void)
{
  /* Current CPU control status */
  ctrl_union CPU_CTRL_reg;
  
  CPU_CTRL_reg.ctrl_t.tswoff_req = 1;
  CPU_CTRL_reg.ctrl_t.tsw_req_ack = 0;

  IOWR_ALT_CTRL_REGISTER(CPU_CTRL_reg.ctrl_integer);

}

/*!----------------------------------------------------------------------------
  Function    : get_new_task
  Description : Retrieves context data on next tasks (used when
                contextswitch is performed manually).
  Arguments   : none
  Return      : none
-----------------------------------------------------------------------------*/
void get_new_task(void)
{
   /* New task to start is fetched from NEXT_TASKID */
  RUNNING_TASKID = NEXT_TASKID;
   /* Point to new tasks tcb */
  current_tcb = &TCB_LIST[RUNNING_TASKID];

#ifdef DEF_LOGGING_PRINT  
printf("\nTSW; Task blocked/ready and new taskid = %d, Time = %d\n", RUNNING_TASKID, ((TIMER_READ() & 0xFFFFE000)>>13));
//printf("\nTSW; Task blocked/ready and new taskid = %d, Time = %d \n", RUNNING_TASKID, TIMER_READ()); 
#endif

}

/*!----------------------------------------------------------------------------
  Function    : irq_wait
  Description : Waits for interrupt on IRQ level.
  Arguments   : int irq     - irq level

  Return      : none
-----------------------------------------------------------------------------*/
void irq_wait(int IRQ_number)
{
  svc_t           svc;
  statusA_union   statusA;
  int             retval;

  tsw_off();
  svc.irq_wait_t.type =  sierra_irq_wait;
  svc.irq_wait_t.irq_type =  IRQ_number;
  
  /* Start service call*/
  
  IOWR_ALT_SVC_REGISTER(svc.svc_input);

  //  ack_wait
  do {
	statusA.statusA_reg_integer = M_RD_Sierra_statusA_reg;
  } while (statusA.statusA_t.svc_ack == 1);
  
   NEXT_TASKID=constant_task_mask & statusA.statusA_t.svc_return;
//   printf("  NEXTTASK_ID_IRQ = %d\n", NEXT_TASKID); /* testning */
   tsw_on();
   taskswitch; /* perform manual contextswitch */
}
