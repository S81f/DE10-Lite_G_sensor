-- Author: Linus Eriksson
-- Date: 2017-04-23

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

entity TIMER_HW_IP is
	port(
		reset_n : in std_logic;
		clk : in std_logic;
		cs_n : in std_logic;
		addr : in std_logic_vector(1 downto 0);
		write_n : in std_logic;
		read_n : in std_logic;
		din : in std_logic_vector(31 downto 0);
		dout : out std_logic_vector(31 downto 0));
end;

architecture rtl of TIMER_HW_IP is
	signal data_reg : std_logic_vector(31 downto 0);
	signal control_reg : std_logic_vector(1 downto 0);
	
	component timer_top
		port(
			control_timer : in std_logic_vector(1 downto 0);
			clk : in std_logic;
			reset_n : in std_logic;
			timer_data : inout std_logic_vector(31 downto 0));
	end component;
	
begin
	
	bus_register_write_process:
	process(clk,reset_n)
	begin
		if (reset_n = '0') then
			control_reg <= (others => '0');
		elsif (rising_edge(clk)) then
			if(cs_n = '0' and write_n = '0' and addr = "01") then
				control_reg(1 downto 0) <= din(31 downto 30);
			else
				null;
			end if;
		else
			null;
		end if;
	end process;
	
	bus_register_read_process:
	process(cs_n,read_n,addr)
	begin
		if (cs_n = '0' and read_n = '0' and addr = "00") then
			dout <= data_reg;
		else
			dout <= (others => 'X');
		end if;
	end process;
	
	timer_function : timer_top
		port map(
			control_timer 	=> control_reg(1 downto 0),
			clk 				=> clk,
			reset_n 			=> reset_n,
			timer_data 		=> data_reg);
	
end;