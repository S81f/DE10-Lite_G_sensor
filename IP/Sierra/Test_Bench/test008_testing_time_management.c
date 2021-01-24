
/*!----------------------------------------------------------------------
      Sierra Real-Time Kernel test code for NiosII

  Filename      :  test008_testing_time_management.c

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
#include "test008_testing_time_management.h"
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
/*TEST 008.
 *Creates 7 tasks with decending period times. This test makes sure they start in the
 *correct order. Using a test variable and the assert function to test the order they
 *start in. Task 7 acts as idle task for this test to prevent the test to go back
 *to main test file. The test does not test if the exact timing is correct, just that each
 *task starts in the order it should according to the period times.
 *
 */


void test_008_testing_time_management(){
	int test_variable=0;  //to make sure the tasks start in the correct order

	//----------------------------------------------------- Task code
	void task_1_code()
	{
		init_period_time(100);  //the last task to start
		wait_for_next_period();
		assert(test_variable==5);  //makes sure this task starts after task_2
		printf("Test 008, testing time management passed\n"); //prints on console if test passes
		test_variable=6;  //change the test variable to stop the test in task 7
		task_delete(); //deletes the task to free it for other tests



	}
	//-----------------------------------------------------

	void task_2_code(void){

		init_period_time(90);  //set period time
		wait_for_next_period();  //wait for scheduled time
		assert(test_variable==4); //make sure it starts after task 3
		test_variable=5; //change test variable for next task
		task_delete();  //delete the test for other tasks
	}
	//-----------------------------------------------------

	void task_3_code(void){
		init_period_time(80);  //set period time
		wait_for_next_period();  //wait for scheduled time
		assert(test_variable==3);  //make sure this task starts after task 4
		test_variable=4;  //changes the variable for next task
		task_delete();  //deletes the task to free it for other tasks

	}

	//-----------------------------------------------------

	void task_4_code(void){

		init_period_time(70);  //set period time
		wait_for_next_period();  //wait for scheduler
		assert(test_variable==2);  //make sure this task starts after task 5
		test_variable=3; //change test variable for next task
		task_delete(); //delete this task to use for other tests
	}

	//-----------------------------------------------------

	void task_5_code(void){

		init_period_time(60);  //set period time
		wait_for_next_period();  //wait for scheduler
		assert(test_variable==1); //make sure this task starts after task 6
		test_variable=2; //set test variable for next task
		task_delete();  //delete task for other tests

	}
	//-----------------------------------------------------

	void task_6_code(void){

		init_period_time(50);  //set period time
		wait_for_next_period();  //wait for scheduler
		test_variable=1;  //set test variable for next task
		task_delete();  //delete task to free it up for next test

	}
	//-----------------------------------------------------

	void task_7_code(void){  //task 7 is acting as idle task, just passing time and preventing
		//the test to go back to main test file.
		int i;
		for(i=0; i<500000; i++);
		if(test_variable==6){
			task_delete();   //when test is done delete this task to free it for other tests
		}
	}
	//-----------------------------------------------------



	//Create all tasks for testing
	tsw_off();
	task_create(task_2, 6, READY_TASK_STATE, task_2_code, task_2_stack, STACK_SIZE);
	task_create(task_3, 5, READY_TASK_STATE, task_3_code, task_3_stack, STACK_SIZE);
	task_create(task_4, 4, READY_TASK_STATE, task_4_code, task_4_stack, STACK_SIZE);
	task_create(task_5, 3, READY_TASK_STATE, task_5_code, task_5_stack, STACK_SIZE);
	task_create(task_6, 2, READY_TASK_STATE, task_6_code, task_6_stack, STACK_SIZE);
	task_create(task_7, 1, READY_TASK_STATE, task_7_code, task_7_stack, STACK_SIZE);
	task_create(task_1, 7, READY_TASK_STATE, task_1_code, task_1_stack, STACK_SIZE);
	tsw_on();
}
//back to idle task in main_test_file
