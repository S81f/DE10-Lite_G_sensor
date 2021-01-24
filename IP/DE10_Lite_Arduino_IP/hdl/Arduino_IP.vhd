-- Author: Linus Eriksson
-- Date: 2017-04-23

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity arduino_component is
	port(
		ARDUINO_IO : inout std_logic_vector(15 downto 0);
		ARDUINO_UART : out std_logic;

		-- Avalon interface
		reset_n : in std_logic;
		clk 	: in std_logic;
		cs_n 	: in std_logic;
		addr 	: in std_logic_vector(3 downto 0);
		write_n : in std_logic;
		read_n 	: in std_logic;
		din 	: in std_logic_vector(31 downto 0);
		dout 	: out std_logic_vector(31 downto 0));
end;

architecture rtl of arduino_component is
	
	-- Storing I/O values here
	signal in_data_register : std_logic_vector(15 downto 0);
	signal out_data_register : std_logic_vector(15 downto 0);
	signal output_enable_register : std_logic_vector(15 downto 0);
	
begin
	
	-- If a pins output enable is '1', output data on this pin, else dont drive it
	ARDUINO_IO(0) <= out_data_register(0) when (output_enable_register(0) = '1') else 'Z';
	ARDUINO_IO(1) <= out_data_register(1) when (output_enable_register(1) = '1') else 'Z';
	ARDUINO_IO(2) <= out_data_register(2) when (output_enable_register(2) = '1') else 'Z';
	ARDUINO_IO(3) <= out_data_register(3) when (output_enable_register(3) = '1') else 'Z';
	ARDUINO_IO(4) <= out_data_register(4) when (output_enable_register(4) = '1') else 'Z';
	ARDUINO_IO(5) <= out_data_register(5) when (output_enable_register(5) = '1') else 'Z';
	ARDUINO_IO(6) <= out_data_register(6) when (output_enable_register(6) = '1') else 'Z';
	ARDUINO_IO(7) <= out_data_register(7) when (output_enable_register(7) = '1') else 'Z';
	ARDUINO_IO(8) <= out_data_register(8) when (output_enable_register(8) = '1') else 'Z';
	ARDUINO_IO(9) <= out_data_register(9) when (output_enable_register(9) = '1') else 'Z';
	ARDUINO_IO(10)	<= out_data_register(10) when (output_enable_register(10) = '1') else 'Z';
	ARDUINO_IO(11)	<= out_data_register(11) when (output_enable_register(11) = '1') else 'Z';
	ARDUINO_IO(12)	<= out_data_register(12) when (output_enable_register(12) = '1') else 'Z';
	ARDUINO_IO(13)	<= out_data_register(13) when (output_enable_register(13) = '1') else 'Z';
	ARDUINO_IO(14)	<= out_data_register(14) when (output_enable_register(14) = '1') else 'Z';
	ARDUINO_IO(15)	<= out_data_register(15) when (output_enable_register(15) = '1') else 'Z';
	
	-- When the pin is used as output, the CPU should not
	-- be reading from it, trying to do so will return 0.
	-- If the pin is not used as output, read value of pin to register.
	in_data_register(0) <= '0' when (output_enable_register(0) = '1') else ARDUINO_IO(0);
	in_data_register(1) <= '0' when (output_enable_register(1) = '1') else ARDUINO_IO(1);
	in_data_register(2) <= '0' when (output_enable_register(2) = '1') else ARDUINO_IO(2);
	in_data_register(3) <= '0' when (output_enable_register(3) = '1') else ARDUINO_IO(3);
	in_data_register(4) <= '0' when (output_enable_register(4) = '1') else ARDUINO_IO(4);
	in_data_register(5) <= '0' when (output_enable_register(5) = '1') else ARDUINO_IO(5);
	in_data_register(6) <= '0' when (output_enable_register(6) = '1') else ARDUINO_IO(6);
	in_data_register(7) <= '0' when (output_enable_register(7) = '1') else ARDUINO_IO(7);
	in_data_register(8) <= '0' when (output_enable_register(8) = '1') else ARDUINO_IO(8);
	in_data_register(9) <= '0' when (output_enable_register(9) = '1') else ARDUINO_IO(9);
	in_data_register(10)	<= '0' when (output_enable_register(10) = '1') else ARDUINO_IO(10);
	in_data_register(11)	<= '0' when (output_enable_register(11) = '1') else ARDUINO_IO(11);
	in_data_register(12)	<= '0' when (output_enable_register(12) = '1') else ARDUINO_IO(12);
	in_data_register(13)	<= '0' when (output_enable_register(13) = '1') else ARDUINO_IO(13);
	in_data_register(14)	<= '0' when (output_enable_register(14) = '1') else ARDUINO_IO(14);
	in_data_register(15)	<= '0' when (output_enable_register(15) = '1') else ARDUINO_IO(15);
	
	
	bus_register_read_process:
	process(cs_n,read_n,addr) begin
		if ((cs_n = '0') and (read_n = '0')) then
			-- This can only be a adruino_digital_read
			-- The data to return is the value of
			-- in_data_register at index specified by addr
			-- the read value is either high or low
			dout(31 downto 1) <= (others => '0');
			dout(0) <= in_data_register(to_integer(unsigned(addr)));
		else
			dout <= (others => 'X');
		end if;
	end process;
	
	bus_register_write_process:
	process(clk,reset_n) begin
		if (reset_n = '0') then
			-- Default to using inputs and 
			output_enable_register <= (others => '0');
			out_data_register <= (others => '0');
			ARDUINO_UART <= '0';
		elsif (rising_edge(clk)) then
			if ((cs_n = '0') and (write_n = '0')) then
				-- This call can be either 
				-- a pin_mode or digital_write
				-- addr specifies which
				case addr is
					when "0000" =>
						-- arduino_pin_mode call
						-- din(5..1) contains index
						-- din(0) contains on/off
						output_enable_register(to_integer(unsigned(din(4 downto 1)))) <= din(0);
					when "0001" =>
						-- arduino_digital_write call
						out_data_register(to_integer(unsigned(din(4 downto 1)))) <= din(0);
						
					when "0010" =>		
						if(din(0) = '1') then
							output_enable_register(0) <= '0';
							output_enable_register(1) <= '0';
							ARDUINO_UART <= '1';
						else
							ARDUINO_UART <= '0';
						end if;
						
					when others => null;
				end case;
			end if;
		end if;
	end process;
	
end;
