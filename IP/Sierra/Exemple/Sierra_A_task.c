/*!----------------------------------------------------------------------
      Sierra Real-Time Kernel exampel code for NiosII
 
  Filename      :  sierra_one_Task_example.c
  
  Company       :  AGSTU AB (www.agstu.com)
  Date Changed  :  2010
  Description   :  System with one periodic tasks. This example is conected to teching book from AGSTU AB.
  System
  Components    :  Sierra, real-time kernel HW and SW.
  Note          :  Simple example created with the following configurations:
                   - NIOSII/e (economy) or NiosII/s (standard) @ 50 MHz
                   - Quartus 10, 9.1, Nios IDE 9.1
		   - Microsoft XP
                   If you have a small memory or problem with the PC it can help with:
                   - Reduced Device Drivers (System Library Properties)
                   - Small C Library (System Library Properties)
                   
-----------------------------------------------------
  Task Description: 
  There are Idle task and periodic Timer_Task with deadline as the period time (one sec). 
  Idle will run when the other tasks are in BLOCKED state (Witing for next period). 
-------------------------------------------------------
 
COPYRIGHT (C) 2009, 2010 AGSTU AB
All rights reserved. AGSTU's source code is an unpublished work, and the use of a copyright notice does not imply otherwise. This source code contains confidential, trade-secret material of AGSTU AB. Any attempt at or participation in deciphering, decoding, reverse engineering, or in any way altering the source code is strictly prohibited unless the prior written consent of AGSTU AB is obtained.
Disclaimer. All code in this file are provided "as is" and without any warranties expressed or implied, including but not limited to implied warranties of merchantability and fitness for a particular purpose. In no event should the author be liable for any damages whatsoever (including without limitation, damages for loss of business profits, business interruption, loss of business information, or any other pecuniary loss) arising out of the use or inability to use information (including software, designs and files) provided on this site.

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

// TASK STACKS
#define STACK_SIZE 800
char idle_stack[STACK_SIZE];
char timer_task_stack[STACK_SIZE];

//-----------------------------------------------------
void timer_task_code(void)
{
    int temp=0, i = 0;

    printf("Timer Task init\n ");
    init_period_time(50); // one secound period time
    
    while(1) // Loop forever
    { 
      wait_for_next_period(); //Every secound
      //for(i=0; i<500000; i++);  //virtual load
      temp = temp +1;
      printf("\nSec: %d ", temp);
    }
}

//-----------------------------------------------------

void idle_task_code(void){
    /* Never blocked. Idle shall only be in running or ready state, lowest priority and taskid 0 */
    int i=0;
  while(1) // Loop forever!
   {for(i=0; i<500000; i++); // 
    printf(".");
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
task_create(Timer_Task, 1, READY_TASK_STATE, timer_task_code, timer_task_stack, STACK_SIZE); 


// Start the Sierra scheduler 
  tsw_on(); // enable CPU irq from Sierran and now at least idle will give a irq.

  while(1) {
    // Should never end up here...!
    printf ("* ERROR! SYSTEM FAILED *\n "); 
  }
}
