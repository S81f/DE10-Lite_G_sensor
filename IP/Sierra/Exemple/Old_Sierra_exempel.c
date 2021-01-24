/*!----------------------------------------------------------------------
      Sierra Lite Real-Time Kernel for NiosII
 
  Filename      :  sierra_normal_8_example.c
  
  Company       :  Mälardalen University, RealFast (from 1998)
  Prevas AB (from 2006) /AGSTU AB (from 2009)
  Date Created  :  2006-09
  Description   :  Get started system with periodic tasks. 
  System
  Components    :  Sierra_normal_8 real-time kernel
                :  NiosII CPU
                :  UART (for print-outs)
  Note          :  Simple example created with the following configurations:
                   - NiosII/s (standard) @ 50 MHz
                   - Quartus 9.0, Nios IDE 9.0
                   - Reduced Device Drivers (System Library Properties)
                   - Small C Library (System Library Properties)
 
  COPYRIGHT (C) 2006- Prevas AB, 2009 AGSTU AB
 
  All rights reserved. AGSTU's source code is an unpublished work,
  and the use of a copyright notice does not imply otherwise. This
  source code contains confidential, trade-secret material of AGSTU AB.
  Any attempt at or participation in deciphering, decoding, reverse
  engineering, or in any way altering the source code is strictly
  prohibited unless the prior written consent of AGSTU AB is obtained.
-----------------------------------------------------------------------*/

/* Sierra Driver Includes */
#include "altera_avalon_sierra_ker.h"
#include "system.h" // for use of SOPC base-address definitons
#include "stdio.h"
#include "io.h"

// TASK, in Sierra
#define IDLE 0
#define Task1 1
#define Task2 2
#define Task3 3
#define Task4 4
#define Task5 5
#define Task6 6
#define Task7 7

// STATES
#define BLOCKED 0
#define READY 1

// SEMAPHORES
#define SEM_Print 0 //can be used for secure the print resource
#define SEM1 1
#define SEM2 2
#define SEM3 3
#define SEM4 4
#define SEM5 5
#define SEM6 6
#define SEM7 7

// Flags
#define FLAG0 0   //you could call the flag with name
#define FLAG1 1
#define FLAG2 2
#define FLAG3 3
#define FLAG4 4
#define FLAG5 5
#define FLAG6 6
#define FLAG7 7


// TASK STACKS
#define STACK_SIZE 800
char idle_stack[STACK_SIZE];
char stack1[STACK_SIZE];
char stack2[STACK_SIZE];
char stack3[STACK_SIZE];
char stack4[STACK_SIZE];
char stack5[STACK_SIZE];
char stack6[STACK_SIZE];
char stack7[STACK_SIZE];

/*!-----------------------------------------------------
  Task Description:
  
  There are 4 tasks in the system, one Idle task,
  and Task1, Task2 and Task3. 
  
  Task1, Task2 and Task3 are periodic. Task1 and Task3
  have 10 times longer period time than Task2, which will 
  make Task2 run ten times more often. 
  
  Idle will run when the other tasks are in BLOCKED 
  state. 
-------------------------------------------------------*/

void task_info(void)
{
task_info_t info;
    
printf("Idle\n");
info = task_getinfo(IDLE);
printf("  info.state_info = %d\n", info.state_info);
printf("  info.priority = %d\n", info.priority); 

     printf("Task1\n");
info = task_getinfo(Task1);
printf("  info.state_info = %d\n", info.state_info);
printf("  info.priority = %d\n", info.priority);

printf("Task2\n");
info = task_getinfo(Task2);
printf("  info.state_info = %d\n", info.state_info);
printf("  info.priority = %d\n", info.priority);

printf("Task3\n");
info = task_getinfo(Task3);
printf("  info.state_info = %d\n", info.state_info);
printf("  info.priority = %d\n", info.priority);
}

void t1(void)
{
    unsigned int temp = 0;
//    int i=0;
    init_period_time(50);
    printf("Task1 starts.\n ");
    while(1)
    {
      temp = temp +1;
      printf("%d ", temp);  
      wait_for_next_period(); 
    }
}

void t2(void)
{
    unsigned int temp = 0;
    int i=0;
    init_period_time(500); 
  // frequence = 1 Hz
    while(1)
    {
    printf("\nT2 start ");
    temp = temp +1;
    printf("T2 %d ", temp);
//Workload
    for(i=0; i<500000; i++);
    for(i=0; i<500000; i++);
    for(i=0; i<500000; i++);
    for(i=0; i<500000; i++);

    printf("T2 ready");

    wait_for_next_period(); 
    }
}

void t3(void){
    int temp = 0; int i=0;
    
    // frequence = 0,1 Hz
    init_period_time(1000);  
 
    while(1){  
    printf("\nT3 start");
    temp = temp + 1;
    printf("T3 %d", temp);
//    Workload
      for(i=0; i<500000; i++);
     for(i=0; i<500000; i++);
      for(i=0; i<500000; i++);
       for(i=0; i<500000; i++);
        for(i=0; i<500000; i++);

     printf("T3 ready");

    wait_for_next_period();
  }
}



void idle(void){

    /* Never use semaphores in idle. It can never be blocked. 
     * Idle shall always exist and have prio lowest and taskid 0 */
    
    int i=0;
   
    printf ("\nTASK STARTUP SEQUENCE:\n "); 
    printf ("**********************\n "); 
    printf ("* IDLE starts T1-T2. *\n "); 
    printf ("**********************\n "); 
    task_start(Task1);
    task_start(Task2);
    task_start(Task3);

  while(1){
    for(i=0; i<500000; i++);
    for(i=0; i<500000; i++);
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
   * Wanted tick time : 20 ms
   * Formula gives    : 20 ms x 50 MHx / 1000 => 1000(dec)
   * ******************************************************************/
   set_timebase(1000); 
 
  printf("  Sierra timebas = %d\n", SierraTime_base_reg());

  // Initialise Sierra            
 

/*
 * task_create() 
 */
 
//        IDLE TaskID, Lowest Priority = B"11" = 3, State Ready
task_create(IDLE, 0, READY, idle, idle_stack, STACK_SIZE); 
    
//        TaskID, Prio, State
task_create(Task1, 7, BLOCKED, t1, stack1, STACK_SIZE); 

//        TaskID, Prio, State
task_create(Task2, 2, BLOCKED, t2, stack2, STACK_SIZE);

//        TaskID, Prio, State
task_create(Task3, 1, BLOCKED, t3, stack3, STACK_SIZE);

/*
 * task_getinfo()
 */
printf("\nTask_getinfo() on created tasks: \n");
//info();

  // Start the Sierra scheduler 
  tsw_on();

  while(1) {
    // Should never end up here...!
    printf ("**********************\n "); 
    printf ("* ERROR! SYSTEM FAILED *\n "); 
    printf ("**********************\n ");
  }
  return(0);
}
