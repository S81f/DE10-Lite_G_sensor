
/*!----------------------------------------------------------------------
      Sierra Real-Time Kernel test code for NiosII

  Filename      :  test_007_testing_flags.c

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
#include "test007_testing_flags.h"
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
/*TEST 007. This will create 7 tasks and make sure the tasks with higher prio waits for the right
 * flags before starting. The order of the tasks starting should be 5,4,6,3,7,2,1.
 *
 *The combination of flags tested are 0001, 0011, 0111, 1111
 */


void test_007_testing_flags(){
	int test_variable=0;


	//----------------------------------------------------- Task code
	void task_1_code()
	{

		flag_wait(15);   //wait for flags to be 1111
		//printf("1 \n");  //the current task, temporary to see what order they start in
		assert(test_variable==4);  //for stopping test if tasks do not start in right order
		printf("test 007, testing flags passed \n");  //if test pass, write on console
		flag_clear(15);  //clear all flags for future use
		task_delete();  //deletes the task for next test to use


	}
	//-----------------------------------------------------

	void task_2_code(void){

		flag_wait(7);  //wait for flags 0111
		//printf("2 \n"); //the current task, temporary to see what order they start in
		assert(test_variable==3); //for stopping test if tasks do not start in right order
		flag_clear(7);  //clear flags
		test_variable=4; //change test variable
		flag_set(15);  //set flags 1111
		task_delete();  //delete task for future tests

	}
	//-----------------------------------------------------

	void task_3_code(void){

		flag_wait(3); //wait for flags 0011
		//printf("3 \n"); //the current task, temporary to see what order they start in
		assert(test_variable==2);  //for stopping test if tasks do not start in right order
		flag_clear(3);  //clear flags
		task_delete();	//delete task for future tests

	}

	//-----------------------------------------------------

	void task_4_code(void){

		flag_wait(1);  //wait for flag 0001
		//printf("4 \n"); //the current task, temporary to see what order they start in
		assert(test_variable==1); //to stop the test if tasks start in wrong order
		flag_clear(1);	//clear flags
		task_delete();	//delete task

	}

	//-----------------------------------------------------

	void task_5_code(void){

		test_variable=1;  //set test variable
		//printf("5 \n"); //the current task, temporary to see what order they start in
		flag_set(1); //set flags 0001
		task_delete(); //delete task

	}
	//-----------------------------------------------------

	void task_6_code(void){

		test_variable=2; //set test variable
		//printf("6 \n"); //the current task, temporary to see what order they start in
		flag_set(3); //set flags to 0011
		task_delete(); 	//delete task

	}
	//-----------------------------------------------------

	void task_7_code(void){

		test_variable=3; //set test variable
		//printf("7 \n"); //the current task, temporary to see what order they start in
		flag_set(7);  //set flags to 0111
		task_delete(); //delete task for future tests

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
