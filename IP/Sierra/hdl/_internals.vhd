 --------------------------------------------------------------------------------
 --                Sierra Real-Time Kernel
 --             
 --  $Workfile    :  _interenals.vhd $ 
 -- 
 --  Company      :  AGSTU AB
 --  Programmer(s):  Lennart Lindh 
 --               :  Susanna Nordstr�m (SUNO)
 --               :  David Borgedahl (DABL)
 --  Date Created :  2003-02-17 
 --  
 --  Description  :  Constant declarations for sierra vhdl-files. 
 --  Notes        :  
 -- 20160303. Add change_task_id, LLH
 -- 20160501: Version register and separate numbers of semafores
 -- 
 --  COPYRIGHT (C) 2016
 -- 
 -- COPYRIGHT (C) 2016 AGSTU AB
 -- All rights reserved. AGSTU's source code is an unpublished 
 -- work, and the use of a copyright notice does not imply otherwise. This 
 -- source code contains confidential, trade-secret material of AGSTU AB. 
 -- Any attempt at or participation in deciphering, decoding, reverse 
 -- engineering, or in any way altering the source code is strictly 
 -- prohibited unless the prior written consent of AGSTU AB is obtained.
 -- Disclaimer
 -- All code in this file are provided "as is" and without any warranties 
 -- expressed or implied, including but not limited to implied warranties 
 -- of merchantability and fitness for a particular purpose. In no event 
 -- should the author be liable for any damages whatsoever (including without 
 -- limitation, damages for loss of business profits, business interruption, 
 -- loss of business information, or any other pecuniary loss) arising out 
 -- of the use or inability to use information (including software, designs 
 -- and files) provided on this site.
 -- 
 ----------------------------------------------------------------

library ieee;
library work;
use ieee.std_logic_1164.all;
USE ieee.std_logic_unsigned.all;
USE ieee.numeric_std.ALL;

package internals is
  
--- Adress bits to the bus
constant CPU_Adress_enable_bits : integer := 8;  -- Used by Sierra decoder
  
-- Register adresser
-- Register adress declarations 8 bitars, 4 bytes, 32 bits (used only in bif)
  constant sierra_version_addr         : std_logic_vector(CPU_Adress_enable_bits-1 downto 0) := "00000000";  -- 00H
  constant sierra_time_base_addr       : std_logic_vector(CPU_Adress_enable_bits-1 downto 0) := "00000100";  -- 04H
  constant sierra_status_a_addr        : std_logic_vector(CPU_Adress_enable_bits-1 downto 0) := "00001000";  -- 08H
  constant sierra_status_b_addr        : std_logic_vector(CPU_Adress_enable_bits-1 downto 0) := "00001100";  -- 0CH
  constant sierra_service_request_addr : std_logic_vector(CPU_Adress_enable_bits-1 downto 0) := "00010000";  --10H
  constant sierra_control_addr         : std_logic_vector(CPU_Adress_enable_bits-1 downto 0) := "00010100";  -- 14H
  constant sierra_control_sw_reset_addr: std_logic_vector(CPU_Adress_enable_bits-1 downto 0) := "00011000";  -- 18H
  
  constant irq_active_level   : std_logic:= '1'; --to allow platform independence
  constant maxtaskid          : integer  := 12;  -- 4096 tasks.
  constant maxprio            : integer  := 12;  -- 4096 priorities.
  constant nrof_SVC_arg_bits  : integer  := 25;  -- maxl�nden p� anropp, max 25 bitar M�ste fixa till s� sm�ning om

-----------------Configure Sierra Version  -----------------
-- Sierra_8 

  constant nrof_irq_bits   : integer := 1;    -- Used by external interrupts handler (Two task)
  constant taskid          : integer := 3;    -- 8 tasks.
  constant prio            : integer := 3;    -- 8 priorities.
--  constant nrof_semaphores : integer := 4;    -- 8 semaphores. -- best to be mre then #task bits.
  constant nrof_flags      : integer := 3;    -- 8 flags -- same as #task bits
  CONSTANT semid           : integer := 4;    --  8 semaphores. -- best to be mre then #task bits
  
----------  
  CONSTANT cofiguration_base_time_divider : integer := 13;   --  Max 32 bits, Create clock tick frequence
  constant nrof_ticks      : integer := 13;   -- Periodic init max 8191
  constant edv             : std_logic := '1';-- counter = 1, no counter = 0

-- VERSION 9.4.0---------------------------------------------------  
--MAJOR version when you make incompatible changes,
  constant Major_version: std_logic_vector(3 downto 0) := X"9";
--MINOR version when you add functionality in a backwards-compatible manner
	constant MINOR_version: std_logic_vector(3 downto 0) := X"4";
--PATCH version when you make backwards-compatible bug fixes 
	constant PATH_version: std_logic_vector(3 downto 0) := X"1";
  constant Sierra_VHDL_VERSION : std_logic_vector(31 downto 0) := Major_version&MINOR_version&PATH_version&"0000"&std_logic_vector(to_unsigned(semid,8))&std_logic_vector(to_unsigned(taskid,8));
  
----------------------------------------------------------

----------------------------------------------------------

-- Argumentet till Module
  constant MAX_TASKID : std_logic_vector( taskid-1 downto 0 ) := (others => '1');
  constant MAX_PRIO   : std_logic_vector( prio-1 downto 0 )   := (others => '1');
  constant IDLE_PRIO  : std_logic_vector( prio-1 downto 0 )   := (others => '1');

  -- Type definitions
  subtype taskid_vector is std_logic_vector( taskid-1 downto 0 );
  subtype prio_vector   is std_logic_vector( prio-1 downto 0 );
  subtype irq_vector    is std_logic_vector( nrof_irq_bits -1 downto 0 );
  type irq_array        is array (0 to 2**nrof_irq_bits) of taskid_vector;

  constant svctype_length : integer := 3;


  -- Following constant depends on following variables
  -- SCH        2+prio
  -- TMQ        1+nrof_ticks
  -- IRQ        2+taskid
  -- SEM        2+counts
  constant svcret_length     : integer := 1+nrof_ticks;
  constant svcret_length_max : integer := 12;
  constant svcarg_length     : integer := 10;
  
  
  CONSTANT semsvcret      : integer := 1+semid; -- if taskid+1 >= semid+1 

  constant RAM_PTR_DEPTH  : integer := prio;
  constant RAM_PTR_WIDTH  : integer := taskid;
  constant RAM_INFO_DEPTH : integer := taskid;
  constant RAM_INFO_WIDTH : integer := taskid+taskid+prio+2;  -- created+ready

  constant rtu_irq_level  : std_logic_vector(2 downto 0) := "100";

------------------------------------------------------------------------------
-- svc_type
  constant sch_svc : std_logic_vector(svctype_length-1 downto 0) := "000";
  constant irq_svc : std_logic_vector(svctype_length-1 downto 0) := "001";
  constant tmq_svc : std_logic_vector(svctype_length-1 downto 0) := "010";
  constant sem_svc : std_logic_vector(svctype_length-1 downto 0) := "011";  -- �ven f�r flaggor  --
  constant end_svc : std_logic_vector(svctype_length-1 downto 0) := "111";

-- svc_subtype
  
-- Task Handling
  constant sch_create  : std_logic_vector(svctype_length-1 downto 0) := "000";
  constant sch_delete  : std_logic_vector(svctype_length-1 downto 0) := "001";
  constant sch_start   : std_logic_vector(svctype_length-1 downto 0) := "010";
  constant sch_block   : std_logic_vector(svctype_length-1 downto 0) := "011";
  constant sch_yield   : std_logic_vector(svctype_length-1 downto 0) := "100";
  constant sch_getinfo : std_logic_vector(svctype_length-1 downto 0) := "101";
  constant sch_change_prio   : std_logic_vector(svctype_length-1 downto 0) := "110";

-- IRQ Handling
  constant irq_init_IRQ_device : std_logic_vector(svctype_length-1 downto 0) := "100";
  constant irq_init_task       : std_logic_vector(svctype_length-1 downto 0) := "000";
  constant irq_wait            : std_logic_vector(svctype_length-1 downto 0) := "001";
  constant irq_remove          : std_logic_vector(svctype_length-1 downto 0) := "010";
  constant irq_getinfo         : std_logic_vector(svctype_length-1 downto 0) := "011";

-- Sem and flaggs
  constant sem_read        : std_logic_vector(svctype_length-1 downto 0) := "000";
  --constant resource_read : std_logic_vector(svctype_length-1 downto 0) := "000";
  constant flag_wait       : std_logic_vector(svctype_length-1 downto 0) := "001";
  constant flag_set        : std_logic_vector(svctype_length-1 downto 0) := "010";
  constant flag_clear      : std_logic_vector(svctype_length-1 downto 0) := "011";
  constant sem_take        : std_logic_vector(svctype_length-1 downto 0) := "100";
  constant sem_release     : std_logic_vector(svctype_length-1 downto 0) := "101";

-- Time Handling
  constant tmq_delay            : std_logic_vector(svctype_length-1 downto 0) := "000";
  constant tmq_init_period_time : std_logic_vector(svctype_length-1 downto 0) := "001";
  constant tmq_read_timeq       : std_logic_vector(svctype_length-1 downto 0) := "010";
  constant tmq_remove           : std_logic_vector(svctype_length-1 downto 0) := "011";
  constant tmq_wait_next_period : std_logic_vector(svctype_length-1 downto 0) := "100";
  constant tmq_restart_period   : std_logic_vector(svctype_length-1 downto 0) := "110";
  
-- tb constants
  constant simple     : std_logic := '0';
  constant not_simple : std_logic := '1';
  constant ready      : std_logic := '1';
  constant blocked    : std_logic := '0';
  


end internals;




