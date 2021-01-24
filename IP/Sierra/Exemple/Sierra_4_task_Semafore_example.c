/*!----------------------------------------------------------------------
      Sierra Real-Time Kernel exampel code for NiosII
 
  Filename      :  sierra_4_Task_example.c
  
  Company       :  AGSTU AB (www.agstu.com)
  Date Created  :  2009
  Description   :  System with one periodic tasks. 
  System
  Components    :  Sierra, real-time kernel HW and SW.
  Note          :  Simple example created with the following configurations:
                   - NIOSII/e (economy) or NiosII/s (standard) @ 50 MHz
                   If you have a small memory:
                   - Reduced Device Drivers (System Library Properties)
                   - Small C Library (System Library Properties)
                   
-----------------------------------------------------
  Task Description:
Semaphores were invented in 1965 by Dijkstra. They are used for synchronization and
mutual exclusion of tasks. A typical application where semaphores are used is resource
management of shared resources, for example printer and shared data. Sierras semaphore types are
binary, which can assume the values available or occupied.


This section describes the functionality of the semaphore management. The semaphores are used in the system to protect shared resources and for synchronization of different tasks. 
There are 16 binary semaphores available in the Sierra education. A semaphore can have a queue of waiting tasks that is as long as there are tasks in the system. This means that a semaphore can be taken by one task and up to 15 other tasks can be waiting for it. The queue is arranged by task-id numbers. Task with highest id-number in the queue will run when the semaphore becomes available.
The following semaphore handling functions are supported:
•	sem_take
•	sem_release
•	sem_read


The example is only a "clear" demonstration, how to make some printing safe.
 
BUGG: SEM_READ Inte rätt semafore nummer
-------------------------------------------------------
 
COPYRIGHT (C) 2009 AGSTU AB
All rights reserved. AGSTU's source code is an unpublished work, and the use of a copyright notice does not imply otherwise. This source code contains confidential, trade-secret material of AGSTU AB. Any attempt at or participation in deciphering, decoding, reverse engineering, or in any way altering the source code is strictly prohibited unless the prior written consent of AGSTU AB is obtained.
Disclaimer
All code in this file are provided "as is" and without any warranties expressed or implied, including but not limited to implied warranties of merchantability and fitness for a particular purpose. In no event should the author be liable for any damages whatsoever (including without limitation, damages for loss of business profits, business interruption, loss of business information, or any other pecuniary loss) arising out of the use or inability to use information (including software, designs and files) provided on this site.

-----------------------------------------------------------------------*/

/* Sierra Driver Includes, hardware base adresses and also C libary */
#include "altera_avalon_sierra_ker.h"
#include <altera_avalon_sierra_io.h>
#include <altera_avalon_sierra_regs.h>
#include <altera_avalon_sierra_name.h>
#include "system.h" // for use of SOPC base-address definitons
#include "stdio.h"
#include "io.h"

// TASK, in Sierra
#define IDLE 0
#define Timer_Task 1
#define Timer_Task_10 2
#define Timer_Task_0_5 3

// SEMAPHORES
#define SEM1 1
#define SEM2 2


// TASK STACKS
#define STACK_SIZE 800
char idle_stack[STACK_SIZE];
char timer_task_stack[STACK_SIZE];
char timer_task_10_stack[STACK_SIZE];
char timer_task_0_5_stack[STACK_SIZE];

//-----------------------------------------------------
void timer_task_code(void)
{
    int temp=0, i=0;
    printf("Timer Task init\n ");
    init_period_time(25); // one second
    
    while(1)
    { 
      wait_for_next_period(); //Every second
//      for(i=0; i<50000; i++);  //virtual load
      temp = temp +1;
      sem_take(SEM2);     // protect print-outs with semaphore 1
        printf("\nSec: %d ", temp);
      sem_release(SEM2);
    }
}

//-----------------------------------------------------
void timer_task_10_code(void)
{
    sem_info_t sem;
    int i=0;
    printf("Timer Task 10 init\n ");
    init_period_time(500); // every 10 second
    
    while(1)
    { 
      wait_for_next_period(); //Every 10 second
     sem_take(SEM2);     // protect print-outs with semaphore 1
       printf("\n start critical section");
       sem = sem_read(Timer_Task);
       printf("  info status = %d  \n", sem.status);
       for(i=0; i<500000; i++);  //virtual load
       sem = sem_read(Timer_Task_0_5);
       printf("  info status = %d  ", sem.status);
       sem = sem_read(Timer_Task);
       printf("\n  info status = %d  ", sem.status);
       printf("end critical section \n");
     sem_release(SEM2);
    }
}

//-----------------------------------------------------
void timer_task_0_5_code(void)
{
//    int i=0;
    printf("Timer Task 0,5 init\n ");
    init_period_time(25); // half second
    
    while(1)
    { 
      wait_for_next_period(); //Every half second
     sem_take(SEM2);     // protect print-outs with semaphore 1
        printf("b");
     sem_release(SEM2);  
    }
}
//-----------------------------------------------------

void idle_task_code(void){
    /* Never blocked. Idle shall only be in running or ready state, lowest priority and taskid 0 */
    int i=0;
  while(1) // Loop forever!
   {for(i=0; i<500000; i++); // You can do something ...
  }
}

/*!-----------------------------------------------------
    Main
-------------------------------------------------------*/

int main (void)
{  
  Sierra_Initiation_HW_and_SW();
  // Get HW Version
  printf("  Sierra HW version = %d\n", sierra_HW_version());
  printf("  Sierra SW driver version = %d\n", sierra_SW_driver_version());

  /*********************************************************************
   * Initialize time base register.
   * This example     : 50 MHz system-clock
   * Wanted tick time : 20 ms (50Hz)
   * Formula gives    : 20 ms x 50 MHx / 1000 => 1000(dec)
   * ******************************************************************/
   set_timebase(1000); 
         
/* task_create()  */
task_create(IDLE, 0, READY_TASK_STATE, idle_task_code, idle_stack, STACK_SIZE); 
task_create(Timer_Task, 2, READY_TASK_STATE, timer_task_code, timer_task_stack, STACK_SIZE); 
task_create(Timer_Task_10, 1, READY_TASK_STATE, timer_task_10_code, timer_task_10_stack, STACK_SIZE);
task_create(Timer_Task_0_5, 7, READY_TASK_STATE, timer_task_0_5_code, timer_task_0_5_stack, STACK_SIZE);


// Start the Sierra scheduler 
  tsw_on(); // enable CPU irq from Sierran and now at least idle will give a irq.

  while(1) {
    // Should never end up here...!
    printf ("* ERROR! SYSTEM FAILED *\n "); 
  }
}
