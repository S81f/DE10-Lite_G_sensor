#include <altera_avalon_sierra_io.h>
#include <altera_avalon_sierra_regs.h>
#include "altera_avalon_sierra_ker.h"
#include <altera_avalon_sierra_name.h>
#include <altera_avalon_sierra_io.h>
#include <altera_avalon_sierra_tcb.h>
#include <stdio.h>
#include <system.h>
#include <io.h>

void Printf_sierra_HW_version(void)
{ 
  version_register_union test;
  test.version_register_int = sierra_HW_version();
  printf("Version = %d.%d.%d\n", test.version_register.MAJOR_version,test.version_register.MINOR_version,test.version_register.PATCH_version );
  printf("Number of tasks bits = %d\n", test.version_register.number_of_tasks);
printf("Number of semaphore bits = %d\n", test.version_register.number_of_semaphores);
}


void task_info(void)
{
task_info_t info;
    
printf("Idle\n");
info = task_getinfo(0);
printf("  info.state_info = %d\n", info.state_info);
printf("  info.priority = %d\n", info.priority); 

     printf("Task1\n");
info = task_getinfo(1);
printf("  info.state_info = %d\n", info.state_info);
printf("  info.priority = %d\n", info.priority);

printf("Task2\n");
info = task_getinfo(2);
printf("  info.state_info = %d\n", info.state_info);
printf("  info.priority = %d\n", info.priority);

printf("Task3\n");
info = task_getinfo(3);
printf("  info.state_info = %d\n", info.state_info);
printf("  info.priority = %d\n", info.priority);
}