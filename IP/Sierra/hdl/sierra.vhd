--
-- VHDL Entity sierra.vhdl
--
-- Created:
--          by - Lennart Lindh, AGSTU AB
--	for - avalon bus
--
-- Avalon businterface
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
LIBRARY ieee ;
USE ieee.std_logic_1164.all;
USE ieee.numeric_std.all;

							
ENTITY sierra IS
   PORT(
      address      : in  std_logic_vector (7 DOWNTO 0) ;
      clk          : in  std_logic  ;
      chipselect_n : in  std_logic  ;
      read_n       : in  std_logic ;
      writedata    : in  std_logic_vector (31 DOWNTO 0) ; -- din
      extirq_n     : in  std_logic_vector (1 downto 0) ;
      reset_n      : in  std_logic  ;
      write_n      : in  std_logic  ;
      readdata     : out std_logic_vector (31 DOWNTO 0) ; -- dout
      irq          : out std_logic ;                      -- irq_n
-- Runing task id
      external_runing_taskid_info : OUT std_logic_vector(2 DOWNTO 0)
   ); 

END sierra ;

ARCHITECTURE rtl OF sierra IS
-- Component Declarations
COMPONENT sierra_top
   PORT(
      addr     : in  std_logic_vector (7 DOWNTO 0) ;
      clk      : in  std_logic  ;
      cs_n     : in  std_logic  ;
      din      : in  std_logic_vector (31 DOWNTO 0) ;
      extirq_n : in  std_logic_vector (1 downto 0) ;
      reset_n  : in  std_logic  ;
      write_n  : in  std_logic  ;
      dout     : out std_logic_vector (31 DOWNTO 0) ;
      irq_n    : out std_logic	;
      external_data_modules_bif_taskid : out std_logic_vector(2 DOWNTO 0)
   );
END COMPONENT;

BEGIN
   -- Instance port mappings.
   sierra: sierra_top
      PORT MAP ( 
          addr     => address,
          clk      => clk,
          cs_n     => chipselect_n,
          din      => writedata,
          extirq_n => extirq_n,
          reset_n  => reset_n,
          write_n  => write_n,
          dout     => readdata,
          irq_n    => irq	, -- irq signal is active high 
          external_data_modules_bif_taskid => external_runing_taskid_info
	);

end;
