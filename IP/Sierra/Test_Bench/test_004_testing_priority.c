
/*!----------------------------------------------------------------------
      Sierra Real-Time Kernel test code for NiosII

  Filename      :  test_004_testing_priority.c

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


COPYRIGHT (C) 2009, 2010 AGSTU AB
All rights reserved. AGSTU's source code is an unpublished work, and the use of a copyright notice does not imply otherwise. This source code contains confidential, trade-secret material of AGSTU AB. Any attempt at or participation in deciphering, decoding, reverse engineering, or in any way altering the source code is strictly prohibited unless the prior written consent of AGSTU AB is obtained.
Disclaimer
All code in this file are provided "as is" and without any warranties expressed or implied, including but not limited to implied warranties of merchantability and fitness for a particular purpose. In no event should the author be liable for any damages whatsoever (including without limitation, damages for loss of business profits, business interruption, loss of business information, or any other pecuniary loss) arising out of the use or inability to use information (including software, designs and files) provided on this site.

-----------------------------------------------------------------------*/
#include "test004_testing_priority.h"
#include "altera_avalon_sierra_ker.h"
#include "altera_avalon_sierra_io.h"
#include "altera_avalon_sierra_regs.h"
#include "altera_avalon_sierra_name.h"
#include "altera_avalon_sierra_tcb.h"
#include <system.h> // for use of SOPC base-address definitons
#include <stdio.h>
#include <io.h>
#include <assert.h> //for testing, will print errors on Nios II Console if the conditions aren't met.
#include "test_setup.h"
/*TEST 004. This will create 7 tasks and make sure they start in
the correct order according to the priority set when the tasks are created: 1,2,3, etc*/



volatile int test_priority=1;
//for testing priority, each task will add 1 to the variable making sure they
//start in the right order. Starting at 1 to make it match task number.


void test_004_testing_priority(){

	volatile int test_priority=1;
	//for testing priority, each task will add 1 to the variable making sure they
	//start in the right order.

	//----------------------------------------------------- Task code
	void task_1_code()
	{
		assert(test_priority==1);
		test_priority++;			//adding one to priority test
		task_delete();			//deleting the task to free it up for other tests
	}

	//-----------------------------------------------------

	void task_2_code(void){

		assert(test_priority==2);
		test_priority++;			//adding one to priority test
		task_delete();			//deleting the task to free it up for other tests
	}
	//-----------------------------------------------------

	void task_3_code(void){

		assert(test_priority==3); //testing priority
		test_priority++;			//adding one to test variable
		task_delete();			//deleting the task to free it up for other tests
	}

	//-----------------------------------------------------

	void task_4_code(void){

		assert(test_priority==4); //testing priority
		test_priority++;			//adding one to the test variable
		task_delete();			//deleting the task to free it up for other tests
	}

	//-----------------------------------------------------

	void task_5_code(void){

		assert(test_priority==5); //testing priority
		test_priority++;		//adding one to test variable
		task_delete();			//deleting the task to free it up for other tests
	}
	//-----------------------------------------------------

	void task_6_code(void){

		assert(test_priority==6); //testing priority
		test_priority++;			//adding one to test variable
		task_delete();			//deleting the task to free it up for other tests
	}
	//-----------------------------------------------------

	void task_7_code(void){

		assert(test_priority==7); //testing priority
		printf("Test 004, testing all tasks started in priority order, passed. \n"); //to let the user know test passed
		task_delete();			//deleting the task to free it up for other tests
	}
	//-----------------------------------------------------


	//Create all tasks for testing
	tsw_off();
	task_create(task_1, 7, READY_TASK_STATE, task_1_code, task_1_stack, STACK_SIZE);
	task_create(task_2, 6, READY_TASK_STATE, task_2_code, task_2_stack, STACK_SIZE);
	task_create(task_3, 5, READY_TASK_STATE, task_3_code, task_3_stack, STACK_SIZE);
	task_create(task_4, 4, READY_TASK_STATE, task_4_code, task_4_stack, STACK_SIZE);
	task_create(task_5, 3, READY_TASK_STATE, task_5_code, task_5_stack, STACK_SIZE);
	task_create(task_6, 2, READY_TASK_STATE, task_6_code, task_6_stack, STACK_SIZE);
	task_create(task_7, 1, READY_TASK_STATE, task_7_code, task_7_stack, STACK_SIZE);
	tsw_on();
}
//back to idle task in main_test_file
