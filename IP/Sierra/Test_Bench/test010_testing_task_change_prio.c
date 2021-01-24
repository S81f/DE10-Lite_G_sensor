
/*!----------------------------------------------------------------------
      Sierra Real-Time Kernel test code for NiosII

  Filename      :  test010_testing_change_prio();

  Company       :  AGSTU AB (www.agstu.com)
  Author	:  Lennart Lindh
  Date Changed  :  2016
  Description   :  System to test functions in Sierra.
  System
  Components    :  Sierra, real-time kernel HW and SW.
  Note          :  Simple example created with the following configurations:
                   - NIOSII/e (economy) @ 50 MHz
                   - Quartus and NIOS 15.0
		   - Microsoft 
                   If you have a small memory:
                   - Reduced Device Drivers (System Library Properties)

COPYRIGHT (C) 2009, 2010 AGSTU AB
All rights reserved. AGSTU's source code is an unpublished work, and the use of a copyright notice does not imply otherwise. This source code contains confidential, trade-secret material of AGSTU AB. Any attempt at or participation in deciphering, decoding, reverse engineering, or in any way altering the source code is strictly prohibited unless the prior written consent of AGSTU AB is obtained.
Disclaimer
All code in this file are provided "as is" and without any warranties expressed or implied, including but not limited to implied warranties of merchantability and fitness for a particular purpose. In no event should the author be liable for any damages whatsoever (including without limitation, damages for loss of business profits, business interruption, loss of business information, or any other pecuniary loss) arising out of the use or inability to use information (including software, designs and files) provided on this site.
-----------------------------------------------------------------------*/

#include "test010_testing_task_change_prio.h"
#include "altera_avalon_sierra_ker.h"
#include "altera_avalon_sierra_io.h"
#include "altera_avalon_sierra_regs.h"
#include "altera_avalon_sierra_name.h"
#include "altera_avalon_sierra_tcb.h"
#include <system.h> // for use of SOPC base-address definitions
#include <stdio.h>
#include <io.h>
#include <assert.h> //for testing, will print errors on Nios II Console if the conditions aren't met.
#include "test_setup.h"

//TEST 010, testing that change prio change the order they executes


void test_010_testing_change_prio(){
	task_info_t info;
	volatile int test_priority=0;

	void task_1_code(void){
		assert(test_priority==0);
		test_priority++;			//adding one to priority test
		// changing order from task 1, 2, 3, 4, 5, 6, 7 till 1, 3, 2, 6, 5, 4, 7
		task_change_prio(task_3, 6);
		task_change_prio(task_2, 5); //task_change_prio(int taskID, int priority)
		task_change_prio(task_6, 4);
		task_change_prio(task_5, 3);
		task_change_prio(task_4, 2);
		task_change_prio(task_7, 1);

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

		assert(test_priority==1);
		test_priority++;			//adding one to priority test
		task_delete();			//deleting the task to free it up for other tests
	}

	//-----------------------------------------------------

	void task_4_code(void){

		assert(test_priority==5);
		test_priority++;			//adding one to priority test
		task_delete();			//deleting the task to free it up for other tests
	}
	//-----------------------------------------------------

	void task_5_code(void){

		assert(test_priority==4);
		test_priority++;			//adding one to priority test
		task_delete();			//deleting the task to free it up for other tests
	}
	//-----------------------------------------------------

	void task_6_code(void){
		assert(test_priority==3);
		test_priority++;			//adding one to priority test
		task_delete();			//deleting the task to free it up for other tests
	}
	//-----------------------------------------------------

	void task_7_code(void){
		assert(test_priority==6);
		printf("Test 010, testing task_change_prio passed \n"); //if the test gets to this point test has passed
		task_delete();					 //delete the task to free it for other tests
	}

	// Create all tasks for testing. To add more tasks just keep adding here and in
	//test_setup.h

	tsw_off(); //shut down the scheduler to create all new tests
	task_create(task_1, 7, READY_TASK_STATE, task_1_code, task_1_stack, STACK_SIZE);
	task_create(task_2, 6, READY_TASK_STATE, task_2_code, task_2_stack, STACK_SIZE);
	task_create(task_3, 5, READY_TASK_STATE, task_3_code, task_3_stack, STACK_SIZE);
	task_create(task_4, 4, READY_TASK_STATE, task_4_code, task_4_stack, STACK_SIZE);
	task_create(task_5, 3, READY_TASK_STATE, task_5_code, task_5_stack, STACK_SIZE);
	task_create(task_6, 2, READY_TASK_STATE, task_6_code, task_6_stack, STACK_SIZE);
	task_create(task_7, 1, READY_TASK_STATE, task_7_code, task_7_stack, STACK_SIZE);
	tsw_on(); //start the scheduler again
}
