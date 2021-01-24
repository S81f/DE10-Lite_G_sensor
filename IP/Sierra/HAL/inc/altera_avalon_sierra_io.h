
#ifndef __ALTERA_AVALON_SIERRA_IO_H__
#define __ALTERA_AVALON_SIERRA_IO_H__
/*!----------------------------------------------------------------------
                  Sierra Real-Time Kernel
                         for NiosII
 
  Filename      :  sierra_avalon_sierra_io.h
  
  Company       :  AGSTU AB
  Date Created  :  2006
  Description   :  This file contains: 
                :  Defines used by service call functions.
                :  CPU dependent structures
                :  - task control block structure (tcb),
                :  - structures for service call communication over CPU bus
  Note          :  
 
COPYRIGHT (C) 2016 AGSTU AB
All rights reserved. AGSTU's source code is an unpublished work, and the use of a copyright notice does not imply otherwise. This source code contains confidential, trade-secret material of AGSTU AB. Any attempt at or participation in deciphering, decoding, reverse engineering, or in any way altering the source code is strictly prohibited unless the prior written consent of AGSTU AB is obtained.
Disclaimer
All code in this file are provided "as is" and without any warranties expressed or implied, including but not limited to implied warranties of merchantability and fitness for a particular purpose. In no event should the author be liable for any damages whatsoever (including without limitation, damages for loss of business profits, business interruption, loss of business information, or any other pecuniary loss) arising out of the use or inability to use information (including software, designs and files) provided on this site.
20101103: Change periodic starts to 16 bits (not to few)

    Encoding of service calls ( 6 bits; 15 downto 10)
-------------------------------------------------------------------------*/

#define sierra_task_create            0x0
#define sierra_task_delete            0x1
#define sierra_task_start             0x2
#define sierra_task_block             0x3
#define sierra_task_yield             0x4
#define sierra_task_getinfo           0x5
#define sierra_task_change_prio       0x6

#define sierra_irq_init               0x8
#define sierra_irq_wait               0x9
#define sierra_irq_remove             0xA
#define sierra_irq_getinfo            0xB
#define sierra_irq_on_off             0xC

#define sierra_delay                  0x10
#define sierra_init_period_time       0x11
#define sierra_read_timeq             0x12
#define sierra_undelay                0x13
#define sierra_wait_for_next_period   0x14
#define sierra_stop_period            0x15
#define sierra_restart_period         0x16

#define sierra_sem_read               0x18

#define sierra_flag_wait              0x19
#define sierra_flag_set               0x1a
#define sierra_flag_clear             0x1b

#define sierra_sem_take               0x1c
#define sierra_sem_release            0x1d
#define sierra_sem_read               0x18



/*!-----------------------------------------------------------------------
    Register interface for communication CPU-Sierra
-------------------------------------------------------------------------*/
// TASK STATES
#define BLOCKED_TASK_STATE 0
#define READY_TASK_STATE 1

/*!-----------------------------------------------------------------------
    Register interface for communication CPU-Sierra
-------------------------------------------------------------------------*/

/* CPU Status register type def. */


/*!-----------------------------------------------------------------------
    Version Register
-------------------------------------------------------------------------*/
typedef union {
struct {
   unsigned short int version_number_of_tasks :8;
   unsigned int :20;
   unsigned short int year :14;
   } ctrl_t;

int version_integer;

} version_union; /* End of union */
/*!-----------------------------------------------------------------------
    Status Register A
-------------------------------------------------------------------------*/

typedef union {
struct {
   unsigned short int svc_ack:1;
   unsigned short int :1;
   unsigned int svc_return:14;
   unsigned int not_used    : 16;
} statusA_t;

int statusA_reg_integer;

} statusA_union; /* End of union */

/*!-----------------------------------------------------------------------
    Status Register B
-------------------------------------------------------------------------*/
typedef union {
struct {
   unsigned int tsw_req_flag:1;
   unsigned int running_taskID:16; 
   unsigned int :4;              
   unsigned int not_used:11;
} statusB_t;

int statusB_reg_integer;

} statusB_union; /* End of union */


/*!-----------------------------------------------------------------------
    Control Register
-------------------------------------------------------------------------*/
typedef union {
struct {
   unsigned short int tsw_req_ack:1;
   unsigned short int tswoff_req:1;
   unsigned short int :14;
   unsigned int not_used:16;
} ctrl_t;

int ctrl_integer;

} ctrl_union; /* End of union */

/*!-----------------------------------------------------------------------
    Version Register
-------------------------------------------------------------------------*/
typedef union {
struct{
  unsigned int number_of_tasks:8;
  unsigned int number_of_semaphores:8;
  unsigned int state_info:4;
  unsigned int PATCH_version:4;
  unsigned int MINOR_version:4;
  unsigned int MAJOR_version:4;
}version_register;
unsigned  int version_register_int;
}version_register_union;


/*!-----------------------------------------------------------------------
    Reset Control Register -- is not defined
-------------------------------------------------------------------------*/

/*!-----------------------------------------------------------------------
    Data structures for reading information from Sierra
-------------------------------------------------------------------------*/
  /*!------------------------------
    task_info_t
  
typedef struct{
  unsigned int state_info:2;
  unsigned int priority:16;
  int : 14;
}task_info_t;
---------------------------------*/
typedef struct{
  unsigned int state_info:2;
  unsigned int priority:14;
}task_info_t;


  /*!------------------------------
    task_periodic start
  ---------------------------------*/
typedef union {
struct {
  unsigned int deadline_control:1;
  int task_id:16;
  int : 15;
}task_periodic_start_t;

unsigned  int periodic_start_integer;

}task_periodic_start_union;

  /*!------------------------------
    sem_info_t
  ---------------------------------*/

typedef struct{

  unsigned int status:1;
  unsigned int semID:12;
  unsigned int :19;
}sem_info_t;//adjusted for v4.01


/*!-----------------------------------------------------------------------
    Service call argument type definitions
    Servie Call Register, 32 bits
-------------------------------------------------------------------------*/
typedef union {
  /* Thread management */
  
  /*!------------------------------
    task_create
  ---------------------------------*/
   struct {
     unsigned int taskID:12;   
     unsigned int priority:12;
     unsigned int state:1;
     unsigned int :1;
     unsigned short int type:6;
   } task_create;

  /*!------------------------------
    task_delete
  ---------------------------------*/
   struct {
      unsigned int :5;
      unsigned int :21;
      unsigned int type:6;
  } task_delete;

  /*!------------------------------
    task_start
  ---------------------------------*/

  struct {
     unsigned int taskID:12; 
     unsigned int :14;       
     unsigned int type:6;
  } task_start;

  /*!------------------------------
    task_block
  ---------------------------------*/
  struct {
     unsigned int taskID:12; 
     unsigned int :14;       
     unsigned int type:6;
  } task_block;

  /*!------------------------------
    task_yield
  ---------------------------------*/
  struct {
     unsigned int :26;
     unsigned int type:6;
  } task_yield;

  /*!------------------------------
    task_change_prio
  ---------------------------------*/
   struct {
     unsigned int taskID:12;   
     unsigned int priority:12;
     unsigned short int type:8;
   } task_change_prio;

  
  /*!------------------------------
    task_getinfo
  ---------------------------------*/

  struct {
      unsigned int taskID:12; 
      unsigned int :14;        
     unsigned int type:6;
  } task_getinfo;

  
  /* Interrupt management */
  
  struct {
     int irq_type:2;
     unsigned int :16;
     unsigned int :8;
     unsigned int type:6;
  } irq_wait_t;

  /* Time management */

    struct {
     unsigned int nroftick:26;
     unsigned int type:6;
  } delay;

  struct {
    unsigned short int taskID:4; 
    unsigned short int :6;
    unsigned short int type:6;
  } undelay;

struct {
     unsigned int period:16; 
     unsigned int :10;
     unsigned short int type:6;
  } init_period_time;

  struct {
    unsigned short int taskID:4; 
    unsigned short int :6;
    unsigned short int type:6;
  } read_timeq;

    struct {
     unsigned int :16;
     unsigned int :10;
     unsigned short int type:6;
  } wait_for_next_period;

  struct {
     unsigned short int taskID:4; 
     unsigned short int :6;
     unsigned short int type:6;
  } stop_period;

  struct {
     unsigned short int taskID:4; 
     unsigned short int :6;
     unsigned short int type:6;
  } restart_period;

/* 
 *      Semaphore management 
 */

  /*!------------------------------
    take_sem
  ---------------------------------*/

  struct {
     unsigned  int semid:12;
     unsigned  int :14;
     unsigned short int type:6;
  } take_sem;


  /*!------------------------------
    release_sem
  ---------------------------------*/

  struct {
     unsigned  int semid:12;
     unsigned  int :14;
     unsigned short int type:6;
  } release_sem;

  /*!------------------------------
    read_sem
  ---------------------------------*/

  struct {
     unsigned int taskID:12;
     unsigned int :14;
     unsigned short int type:6;
  } read_sem;


  /*!------------------------------
    flag_wait
  ---------------------------------*/

  struct {
     unsigned int flag_mask:12; 
     unsigned int :14;
     unsigned short int type:6;
  } flag_wait;


  /*!------------------------------
    flag_set
  ---------------------------------*/

  struct {
     unsigned int flag_mask:12;
     unsigned int :14;
     unsigned short int type:6;
  } flag_set;

  
  /*!------------------------------
    flag_clear
  ---------------------------------*/

  struct {
     unsigned int flag_mask:12; 
     unsigned int :14;
     unsigned short int type:6;
  } flag_clear;

int svc_input;

} svc_t; /* End of union */



#endif /* __ALTERA_AVALON_SIERRA_IO_H__ */

