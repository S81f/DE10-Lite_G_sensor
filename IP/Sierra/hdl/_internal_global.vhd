 --------------------------------------------------------------------------------
 --                Sierra Real-Time Kernel
 --             
 --  $Workfile    :  _internal_global.vhd $ 
 -- 
 --  Company      :  AGSTU AB
 --  Programmer(s):  Lennart Lindh 
 --  Date Created :  2014-12-04
 --  
 --  Description  :  VHDL Package Header 
 --               :  This is global signal and RAM memory for tes cases and syntesis.
 --  Notes        :  Do not change these constants whithout having access to the
 --               :  complete vhdl source code. 
 -- 
 -- COPYRIGHT (C) 2006 - 2014 AGSTU AB
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
 --------------------------------------------------------------------------------
 -- Revision history    
--------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
USE work.internals.all; 

package internal_global is
  
-- Simulation declarations from BIF component
signal time_base_register, time_base_reg_local_var, time_base_reg_local : std_logic_vector(cofiguration_base_time_divider downto 0); 
signal STATUS_register_A        : std_logic_vector(31 downto 0 );
signal STATUS_register_B        : std_logic_vector(31 downto 0 ); 

-- Simulation declarations from SCH component


end internal_global;




