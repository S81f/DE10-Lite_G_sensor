
#ifndef __ALTERA_AVALON_SIERRA_KER_H__
#define __ALTERA_AVALON_SIERRA_KER_H__

/*!----------------------------------------------------------------------
                  Sierra Real-Time Kernel
                         for NiosII
 
  Filename      :  sierra_avalon_sierra_ker.h
  
  Company       :  AGSTU AB
  Date Created  :  2006-08
  Description   :  This file contains: 
                :  Declaration of service call functions.
  Note          :  
20150116: add constant_task_mask, to get a safe task number.
NEXT_TASKID = constant_task_mask & statusB.statusB_t.running_taskID;
20160307: Add task_change_prio (ver 9.2)
20180311: Add on/off LOGGING print function "#define DEF_LOGGING_PRINT"


COPYRIGHT (C) 2009-2015 AGSTU AB
All rights reserved. AGSTU's source code is an unpublished work, and the use of a copyright notice does not imply otherwise. This source code contains confidential, trade-secret material of AGSTU AB. Any attempt at or participation in deciphering, decoding, reverse engineering, or in any way altering the source code is strictly prohibited unless the prior written consent of AGSTU AB is obtained.
Disclaimer
All code in this file are provided "as is" and without any warranties expressed or implied, including but not limited to implied warranties of merchantability and fitness for a particular purpose. In no event should the author be liable for any damages whatsoever (including without limitation, damages for loss of business profits, business interruption, loss of business information, or any other pecuniary loss) arising out of the use or inability to use information (including software, designs and files) provided on this site.

    Number of tasks
-----------------------------------------------------------------
Maxium number of tasks that will be used depend of the hardware version you have. If less than 16
tasks will be used, memory space is saved if this variable is set to correct number as each task allocates memory for its tcb (about 160 bytes/task, see the configuration of the Sierra you use.
*/

#define N_TASKS 8
#define constant_task_mask 0x7
#define C_Sierra_SW_driver_version 100315

/*---------------------------------------------------------------
LOGGING function
- print logging of functions (it will have a havy probe effect on the system, use for pedagogic reasons)
#define DEF_LOGGING_PRINT
*/
//#define DEF_LOGGING_PRINT

/*------------------------------------------------------------------------
    DONT MAKE CHANGES BEYOND THIS LINE
------------------------------------------------------------------------*/
/* Include RTU I/O description */
#include <altera_avalon_sierra_io.h>

/* Include TCB description */
#include <altera_avalon_sierra_tcb.h>

/* Thread context (TCB) data */
extern tcb_t TCB_LIST[N_TASKS];

/* 
 * Context data for task 
 */
extern volatile unsigned char RUNNING_TASKID;
extern volatile unsigned char NEXT_TASKID;

// macros..
// NiosII
#define taskswitch  __asm__("trap"); 

void HW_CTX_SWITCH(void);

/*!-----------------------------------------------------------------------
    Functions for information
-------------------------------------------------------------------------*/
extern unsigned int sierra_HW_version(void);
extern unsigned int sierra_SW_driver_version(void);
extern void set_timebase(unsigned int hex);
extern unsigned int SierraTime_base_reg(void);
extern void print_SierraTime_base_reg();

/*!-----------------------------------------------------------------------
    Kernel help function
-------------------------------------------------------------------------*/
extern void tsw_on(void);
extern void tsw_off(void);
extern void Sierra_Initiation_HW_and_SW(void);
extern void ack_intc(void);
extern void get_new_task(void);
extern void get_next_task(void);

/*!-----------------------------------------------------------------------
    Task handling
-------------------------------------------------------------------------*/
extern void task_create(int taskID, int priority, int tasktate, void (*taskptr)(void), void *stackptr, int stacksz);
extern void task_delete(void);
extern void task_start(int taskID);
extern void task_block(int taskID);
extern void task_yield(void);
extern void task_change_prio(int taskID, int priority);
extern task_info_t task_getinfo(int taskID);


/*!-----------------------------------------------------------------------
    Interrupt handling
-------------------------------------------------------------------------*/
//extern void irq_on_off(int on_off);
//extern void irq_init(unsigned int irq, int edge_level, int hi_lo);
extern void irq_wait(int IRQ_number);
//extern void irq_remove(int IRQ);
//extern irq_info_t irq_getinfo(int IRQ);

/*!-----------------------------------------------------------------------
    Semaphore handling
-------------------------------------------------------------------------*/
extern void sem_take(int semid);
extern void sem_release(int semid);
extern sem_info_t sem_read(int taskID);

/*!-----------------------------------------------------------------------
    Flags
-------------------------------------------------------------------------*/
extern void flag_wait(int flag_mask);
extern void flag_set(int flag_mask);
extern void flag_clear(int flag_mask);

/*!-----------------------------------------------------------------------
    Time handling
-------------------------------------------------------------------------*/
extern void delay(int delay_time);
//extern void undelay(int taskID);
extern void init_period_time(int per_time);
//extern timeq_info_t read_timeq(int taskID);
//extern void remove_from_timeq(int taskID);
extern task_periodic_start_union wait_for_next_period(void);
//extern void stop_period(int taskID);
//extern void start_period(int taskID);
//extern void restart_period(int taskID);

/*!-----------------------------------------------------------------------
    Other
-------------------------------------------------------------------------*/


#endif /* __ALTERA_AVALON_SIERRA_KER_H__ */


