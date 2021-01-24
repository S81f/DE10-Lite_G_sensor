
/*!----------------------------------------------------------------------
      Sierra Real-Time Kernel test code for NiosII

  Filename      :  test006_testing_sem_take_and_release.c

  Company       :  AGSTU AB (www.agstu.com)
  Author		:  Maya Isaksson (isaksson.maya@gmail.com)
  Date Changed  :  2015
		20161031: Change so all semafores are free when you go to next test case
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

#include "test006_testing_sem_take_and_release.h"
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

/*TEST 006.
 * Testing that tasks wait for semaphore before starting up.
 * Not all test cases is tested. Waiting for all semaphore, half of the semaphores, the
 * other half of the semaphores and for the first and last semaphores.
 *
 * Test creates 5 tasks, task 1 is created in blocked state to make task 2 start test.
 *
 */

//defining semaphores for testing, 8 in total (max number of semaphores for this version of Sierra)
#define SEM1 0
#define SEM2 1
#define SEM3 2
#define SEM4 3
#define SEM5 4
#define SEM6 5
#define SEM7 6
#define SEM8 7


void test_006_testing_sem_take_and_release(){

	int test_failed_variable=0;  //used to stop test if test fails.

	//----------------------------------------------------- Task code


	void task_1_code()  //waits for sem 1,3,5,7 should start after task 3.
	//releases sem 2,4,6,8
	{
		assert(test_failed_variable==2); //if test fails here the tasks do not start in the correct order
		//waiting for these semaphores
		sem_take(SEM1);
		sem_take(SEM3);
		sem_take(SEM5);
		sem_take(SEM7);
		//releases these semaphores
		sem_release(SEM2);
		sem_release(SEM4);
		sem_release(SEM6);
		sem_release(SEM8);
		test_failed_variable=3;  //changes the variable for next task
		task_delete();			//delete task for next test to use


	}
	//-----------------------------------------------------

	//task_2 takes all semaphores before starting up task_3, then the task is deleted

	void task_2_code(void){
		assert(test_failed_variable==0); //this should be the first task to start,
		//if it fails here another task takes the semaphores before this
		//takes all semaphores
		sem_take(SEM1);
		sem_take(SEM2);
		sem_take(SEM3);
		sem_take(SEM4);
		sem_take(SEM5);
		sem_take(SEM6);
		sem_take(SEM7);
		sem_take(SEM8);

		test_failed_variable=1;			//changes the test variable for next task
		task_delete();					//delete task for next test to use
	}
	//-----------------------------------------------------


	void task_3_code(void){

		assert(test_failed_variable==1);		//if test fails here the other tasks don't wait
		//for the semaphores to be released before starting
		//releases semaphores 1,3,5,7
		sem_release(SEM1);
		sem_release(SEM3);
		sem_release(SEM5);
		sem_release(SEM7);

		test_failed_variable=2;		//changes test variable for next task
		task_start(task_1);			//starting task_1 that was created as blocked
		task_delete();				//deletes task for next test to use
	}
	//-----------------------------------------------------


	void task_4_code(void){

		assert(test_failed_variable==3); //if test fails here the tasks don't start in the right order
		//waits for semaphores 2,4,6,8
		sem_take(SEM2);
		sem_take(SEM4);
		sem_take(SEM6);
		sem_take(SEM8);
		//releases semaphore 1, 8
		sem_release(SEM1);
		sem_release(SEM8);

		test_failed_variable=4; //changes the test variable for the next task
		task_delete();			//deletes the task for the next test to use
	}

	void task_5_code(void){
		//int all_tests_passed_without_errors=1;
		//temporary, just for stopping the test when it's done
		assert(test_failed_variable==4);		//if test fails here the tasks do not start in the correct order
		//waits for semaphores 1 and 8
		sem_take(SEM1);
		sem_take(SEM8);
		printf("Test 006, testing sem_take and sem_release passed \n"); //lets you know test passed
		//assert(all_tests_passed_without_errors==0); //to stop the tests, all is well
		sem_release(SEM1);
		sem_release(SEM2);
		sem_release(SEM3);
		sem_release(SEM4);
		sem_release(SEM5);
		sem_release(SEM6);
		sem_release(SEM7);
		sem_release(SEM8);
		task_delete(); //deletes the task for next test to use
	}


	//Create all tasks for testing, all put in blocked state except for the first.

	task_create(task_1, 7, BLOCKED_TASK_STATE, task_1_code, task_1_stack, STACK_SIZE);
	task_create(task_2, 6, READY_TASK_STATE, task_2_code, task_2_stack, STACK_SIZE);
	task_create(task_3, 5, READY_TASK_STATE, task_3_code, task_3_stack, STACK_SIZE);
	task_create(task_4, 4, READY_TASK_STATE, task_4_code, task_4_stack, STACK_SIZE);
	task_create(task_5, 3, READY_TASK_STATE, task_5_code, task_5_stack, STACK_SIZE);

}
//back to idle task in main_test_file
