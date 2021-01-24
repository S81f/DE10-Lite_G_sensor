

/*!----------------------------------------------------------------------
      Sierra Real-Time Kernel test code for NiosII

  Filename      :  main_test_file.c

  Company       :  AGSTU AB (www.agstu.com)
  Author		:  Maya Isaksson (isaksson.maya@gmail.com)
  Date Changed  :  2015
  Description   :  System to test functions in Sierra.
  System
  Components    :  Sierra, real-time kernel HW and SW.
  Note          :  Simple example created with the following configurations:
                   - NIOSII/e (economy) or NiosII/s (standard) @ 50 MHz
                   - Quartus 10, 9.1, Nios IDE 10, 9.1
		   - Microsoft XP
                   If you have a small memory:
                   - Reduced Device Drivers (System Library Properties)
Använder assert: för mera information se: http://ptolemy.eecs.berkeley.edu/~johnr/tutorials/assertions.html

-----------------------------------------------------
  Description:
  This is the main file used for testing the different function calls in the Sierra. It
  uses the idle task to start one test after another until all is done. Each test will
  let you know it passed by writing on the Nios II console. If a test fails you will be
  informed of which one failed and on which line, making it easy to find the error.
  To add tasks you need to add task stacks in this file. You also need to define them
  in test_setup.h where you also can change the stack size. You have to create the
  extra tasks in each test file you want to run too. If you just want to run
  certain tests you can comment out the others in this files inside the Idle task.

-------------------------------------------------------

COPYRIGHT (C) 2009, 2010 AGSTU AB
All rights reserved. AGSTU's source code is an unpublished work, and the use of a copyright notice does not imply otherwise. This source code contains confidential, trade-secret material of AGSTU AB. Any attempt at or participation in deciphering, decoding, reverse engineering, or in any way altering the source code is strictly prohibited unless the prior written consent of AGSTU AB is obtained.
Disclaimer
All code in this file are provided "as is" and without any warranties expressed or implied, including but not limited to implied warranties of merchantability and fitness for a particular purpose. In no event should the author be liable for any damages whatsoever (including without limitation, damages for loss of business profits, business interruption, loss of business information, or any other pecuniary loss) arising out of the use or inability to use information (including software, designs and files) provided on this site.

-----------------------------------------------------------------------*/


/* Sierra Driver Includes, hardware base adresses and also C libary */
#include "altera_avalon_sierra_ker.h"
#include "altera_avalon_sierra_io.h"
#include "altera_avalon_sierra_regs.h"
#include "altera_avalon_sierra_name.h"
#include "altera_avalon_sierra_tcb.h"
#include <system.h> // for use of SOPC base-address definitons
#include <stdio.h>
#include <io.h>
#include <assert.h> //for testing, will print errors on Nios II Console if the conditions aren't met.
//All test files and test_setup.h needs to be included
#include "test_001_testing_task_create.h"
#include "test_002_testing_task_delete.h"
#include "test003_testing_task_block.h"
#include "test004_testing_priority.h"
#include "test005_testing_task_start.h"
#include "test006_testing_sem_take_and_release.h"
#include "test007_testing_flags.h"
#include "test008_testing_time_management.h"
#include "test009_testing_delay.h"
#include "test010_testing_task_change_prio.h"
#include "test011_testing_task_block.h"
#include "test012_testing_task_change_prio_same.h"
#include "test_setup.h"

// defining the idle task, in Sierra
#define IDLE 0

// TASK STACKS
//for more than 8 tasks, add task stacks here and in task_setup.h

char idle_stack[STACK_SIZE];
char task_1_stack[STACK_SIZE];
char task_2_stack[STACK_SIZE];
char task_3_stack[STACK_SIZE];
char task_4_stack[STACK_SIZE];
char task_5_stack[STACK_SIZE];
char task_6_stack[STACK_SIZE];
char task_7_stack[STACK_SIZE];


//All tests runs from the idle task. Comment out the ones you don't want to run.
void idle_task_code(void){
	Printf_sierra_HW_version();
	printf("starting tests \n");
	//for testing task management
	test_001_testing_task_create();
	test_002_testing_task_delete();
	test_003_testing_task_block();
	test_004_testing_priority();
	test_005_testing_task_start();
	test_006_testing_sem_take_and_release();
	//test_007_testing_flags();
	test_008_testing_time_management();
	test_009_testing_delay();
	test_010_testing_change_prio();
	test_011_testing_task_block();
	test_012_testing_change_prio_same();
	printf("All tests passed without errors\n");  //all tests has passed if you reach this point.
	while(1); //prevents the tests to start over.
}
/*!-----------------------------------------------------
    Main
-------------------------------------------------------*/

int main (void)
{
	Sierra_Initiation_HW_and_SW();

	/*********************************************************************
	 * Initialize time base register.
	 * This example     : 50 MHz system-clock
	 * Wanted tick time : 20 ms (50Hz)
	 * Formula gives    : 20 ms x 50 MHx / 1000 => 1000(dec)
	 * ******************************************************************/
	set_timebase(1000);



	//Create the idle task here
	task_create(IDLE, 0, READY_TASK_STATE, idle_task_code, idle_stack, STACK_SIZE);
	// Start the Sierra scheduler
	tsw_on();

	while(1) {
		// Should never end up here...!
		printf ("* ERROR! SYSTEM FAILED *\n ");
	}
}




