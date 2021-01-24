/*!----------------------------------------------------------------------
      Sierra Real-Time Kernel exampel code for NiosII
 
  Filename      :  sierra_4_Task_Flag_example.c
  
  Company       :  AGSTU AB (www.agstu.com)
  Date Created  :  2009
  Description   :  System with one periodic tasks. 
  System
  Components    :  Sierra, real-time kernel HW and SW.
  Note          :  Simple example created with the following configurations:
                   - NIOSII/e (economy) or NiosII/s (standard) @ 50 MHz
                   - Quartus 9.0, Nios IDE 9.0
		   - Microsoft XP
                   If you have a small memory:
                   - Reduced Device Drivers (System Library Properties)
                   - Small C Library (System Library Properties)
                   
-----------------------------------------------------
  Task Description:
The Sierra has support for flags for efficient synchronizing of events. 
The entire synchronizing algorithm is handled by the hardware kernel. 
This makes handling of flags very efficient since no valuable CPU time is spent on synchronization. 
The following flag handling functions are supported:
•   flag_wait
•   flag_set
•   flag_clear

-------------------------------------------------------
 
COPYRIGHT (C) 2009, 2010 AGSTU AB
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
#define flag_server_two_task_code_id 1
#define Jobb_task_id 2
#define flag_server_one_task_code_id 3

// SEMAPHORES
#define SEM1 1
#define SEM2 2

// Flags (16 kombinations with 4 flags)
#define FLAG_mask_one 1   //0x0001
#define FLAG_mask_two 2   //0x0010
#define FLAG_mask_three 3   //0x0011

// TASK STACKS
#define STACK_SIZE 800
char idle_stack[STACK_SIZE];
char task_stack_1[STACK_SIZE];
char task_stack_2[STACK_SIZE];
char task_stack_3[STACK_SIZE];

//-----------------------------------------------------
void flag_server_one_task_code(void)
{
    int i=0;
    printf("Flag Server one starts\n ");
    
    while(1)
    { 
     flag_wait(FLAG_mask_two); /* Wait for Flag0 and 
                               Flag1 to be set */

     for(i=0; i<50000; i++);  //virtual load
     flag_clear(FLAG_mask_two);
//   sem_take(SEM2);     // protect print-outs with semaphore 1
     printf("Flag server one ready with flag 1\n");
//   sem_release(SEM2);
     
    }
}

//-----------------------------------------------------
void Jobb_task_code(void)
{
    int i=0;
    printf("Jobb task starts\n ");
    init_period_time(500); // every 5 second
    flag_clear(FLAG_mask_three);
    
    while(1)
    { 
      wait_for_next_period(); //Every second
      
     printf("Set flag 0 and 1\n ");
     flag_set(FLAG_mask_three);  /* Set Flag0 and 
                               Flag1 */
     wait_for_next_period(); //Every second
     printf("Set flag 0\n ");
     flag_set(FLAG_mask_one);  /* Set Flag0 */
      for(i=0; i<500000; i++);  //virtual load

    }
}

//-----------------------------------------------------
void flag_server_two_task_code(void)
{
    int i=0;
    printf("Flag Server two starts\n ");
    
    while(1)
    { 
     flag_wait(FLAG_mask_one); /* Wait for Flag0 to be set */

     for(i=0; i<50000; i++);  //virtual load
     flag_clear(FLAG_mask_one);
//   sem_take(SEM2);     // protect print-outs with semaphore 1
     printf("Flag server two ready with flag 0\n");
//   sem_release(SEM2);
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
task_create(flag_server_two_task_code_id, 1, READY_TASK_STATE, flag_server_two_task_code, task_stack_1, STACK_SIZE); 
task_create(Jobb_task_id, 2, READY_TASK_STATE, Jobb_task_code, task_stack_2, STACK_SIZE);
task_create(flag_server_one_task_code_id, 1, READY_TASK_STATE, flag_server_one_task_code, task_stack_3, STACK_SIZE);


// Start the Sierra scheduler 
  tsw_on(); // enable CPU irq from Sierran and now at least idle will give a irq.

  while(1) {
    // Should never end up here...!
    printf ("* ERROR! SYSTEM FAILED *\n "); 
  }
}

