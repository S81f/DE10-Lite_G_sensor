
/*!----------------------------------------------------------------------
      Sierra Real-Time Kernel test code for NiosII

  Filename      :  test009_testing_delay.c

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
#include "test009_testing_delay.h"
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
/*TEST 009.
 *creates 3 tasks. Delays one to start after the one scheduled to start last and tests the order they
 *start in. Task 3 acts as idle task to prevent the progam to go back to main test file.
 *
 */


void test_009_testing_delay(){
	int test_variable=0;  //to make sure the tasks start in the correct order


	//----------------------------------------------------- Task code
	void task_1_code()
	{
		init_period_time(100);  //without delay this is the last task to start
		wait_for_next_period();  //wait for scheduler
		assert(test_variable==0); //test that this task starts before the other
		test_variable=1; //change test_variable
		task_delete();  //delete task for other tests

	}
	//-----------------------------------------------------

	void task_2_code(void){

		init_period_time(90);  //set period time, should start first without delay
		wait_for_next_period();  //wait for scheduled time
		delay(20); //delay this to start it after the other task
		assert(test_variable==1);  //assert that this starts after the other task
		printf("Test 009 testing delay passed\n");  //prints to console
		test_variable=2;  //change test_variable
		task_delete();  //delete task to use in other tests
	}
	//-----------------------------------------------------

	void task_3_code(void){  //woks like idle task for this test
		int i;
		for(i=0; i<500000; i++);  //just something to do to prevent the test to go back to main test file
		if(test_variable==2){ //when test is done delete this task to fee for other tests
			task_delete();
		}

	}

	//-----------------------------------------------------




	//Create all tasks for testing
	tsw_off();
	task_create(task_1, 7, READY_TASK_STATE, task_1_code, task_1_stack, STACK_SIZE);
	task_create(task_2, 6, READY_TASK_STATE, task_2_code, task_2_stack, STACK_SIZE);
	task_create(task_3, 5, READY_TASK_STATE, task_3_code, task_3_stack, STACK_SIZE);

	tsw_on();
}
//back to idle task in main_test_file
