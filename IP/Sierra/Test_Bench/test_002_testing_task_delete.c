
/*!----------------------------------------------------------------------
      Sierra Real-Time Kernel test code for NiosII

  Filename      :  test_002_testing_task_delete.c

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

#include "test_002_testing_task_delete.h"
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

//TEST 002, testing that all created tasks are in state dormant when deleted
//Creates 7 new tasks and uses the tasks_getinfo function to check that it is running
//deletes the task and after jumping to the next task checks if it is in state dormant.


void test_002_testing_task_delete(){
	task_info_t info;  //used to check the state of tasks

	void task_1_code(void){

		info = task_getinfo(task_1); //store the state info
		assert(info.state_info==0); //check that the state of task is running (0)
		task_delete();  //delete the task to free it for other tests
	}
	//-----------------------------------------------------

	void task_2_code(void){

		info = task_getinfo(task_1);	//store the state info
		assert(info.state_info==3);		//make sure last task is dormant (deleted)
		info = task_getinfo(task_2);	//store the state info
		assert(info.state_info==0);		//check that this task is in state running
		task_delete();					 //delete the task to free it for other tests
	}

	//-----------------------------------------------------

	void task_3_code(void){

		info = task_getinfo(task_2);	//store the state info
		assert(info.state_info==3);		//make sure last task is dormant (deleted)
		info = task_getinfo(task_3);	//store the state info
		assert(info.state_info==0);		//check that this task is in state running
		task_delete();					 //delete the task to free it for other tests
	}

	//-----------------------------------------------------

	void task_4_code(void){

		info = task_getinfo(task_3);	//store the state info
		assert(info.state_info==3);		//make sure last task is dormant (deleted)
		info = task_getinfo(task_4);	//store the state info
		assert(info.state_info==0);		//check that this task is in state running
		task_delete();					 //delete the task to free it for other tests
	}
	//-----------------------------------------------------

	void task_5_code(void){

		info = task_getinfo(task_4);	//store the state info
		assert(info.state_info==3);		//make sure last task is dormant (deleted)
		info = task_getinfo(task_5); 	//store the state info
		assert(info.state_info==0);		//check that this task is in state running
		task_delete();					 //delete the task to free it for other tests
	}
	//-----------------------------------------------------

	void task_6_code(void){
		info = task_getinfo(task_5); 	//store the state info
		assert(info.state_info==3);		//make sure last task is dormant (deleted)
		info = task_getinfo(task_6);	//store the state info
		assert(info.state_info==0);		//check that this task is in state running
		task_delete();					 //delete the task to free it for other tests
	}
	//-----------------------------------------------------

	void task_7_code(void){

		info = task_getinfo(task_6);	//store the state info
		assert(info.state_info==3);		//make sure last task is dormant (deleted)
		info = task_getinfo(task_7);	//store the state info
		assert(info.state_info==0);		//check that this task is in state running
		printf("Test 002, testing task_delete passed \n"); //if the test gets to this point test has passed
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
