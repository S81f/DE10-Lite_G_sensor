-- Author: Linus Eriksson
-- Date: 2017-04-23

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity SSD_IP is
	port(
		-- Avalon interface
		reset_n : in std_logic;
		clk 	: in std_logic;
		cs_n 	: in std_logic;
		addr 	: in std_logic_vector(2 downto 0); -- 3 bits required to address the 7 registers
		write_n : in std_logic;
		read_n 	: in std_logic;
		din 	: in std_logic_vector(31 downto 0);
		dout 	: out std_logic_vector(31 downto 0);
		
		-- Output to SSD
		HEX0 : out std_logic_vector(7 downto 0);
		HEX1 : out std_logic_vector(7 downto 0);
		HEX2 : out std_logic_vector(7 downto 0);
		HEX3 : out std_logic_vector(7 downto 0);
		HEX4 : out std_logic_vector(7 downto 0);
		HEX5 : out std_logic_vector(7 downto 0));
end;

architecture rtl of SSD_IP is

	-- Registers accessible from CPU
	signal HEX_enabled_reg	: std_logic_vector(5 downto 0);	-- 000
	signal HEX0_data_reg	: std_logic_vector(7 downto 0); -- 001
	signal HEX1_data_reg	: std_logic_vector(7 downto 0); -- 010
	signal HEX2_data_reg	: std_logic_vector(7 downto 0); -- 011
	signal HEX3_data_reg	: std_logic_vector(7 downto 0); -- 100
	signal HEX4_data_reg	: std_logic_vector(7 downto 0); -- 101
	signal HEX5_data_reg	: std_logic_vector(7 downto 0); -- 110
	
	-- These values only control the numeric part of the display, decimal point
	-- can be activated by writing to bit 7 in data register.
	type hex_lut_type is array(0 to 15) of std_logic_vector(6 downto 0);
	constant hex_lut : hex_lut_type := (
		"1000000", -- 0
		"1111001", -- 1
		"0100100", -- 2
		"0110000", -- 3
		"0011001", -- 4
		"0010010", -- 5
		"0000010", -- 6
		"1111000", -- 7
		"0000000", -- 8
		"0011000", -- 9
		"0001000", -- A
		"0000011", -- b
		"1000110", -- C
		"0100001", -- d
		"0000110", -- E
		"0001110"  -- F
	);
	
begin

	-- When the enabled bit for the display is set to '0' this outputs all '1' meaning the display is turned off.
	-- Otherwise it concatenates bit 7 from data register with the value obtained from the LUT. The 4 lowest bits
	-- in data reigster is used as an unsigned value as index to the LUT.
	-- Bit 7 in data reg is inverted to make the CPU calls make more sense.
	HEX0 <= "11111111" when (HEX_enabled_reg(0) = '0') else ((not HEX0_data_reg(7 downto 7)) & hex_lut(to_integer(unsigned(HEX0_data_reg(3 downto 0)))));
	HEX1 <= "11111111" when (HEX_enabled_reg(1) = '0') else ((not HEX1_data_reg(7 downto 7)) & hex_lut(to_integer(unsigned(HEX1_data_reg(3 downto 0)))));
	HEX2 <= "11111111" when (HEX_enabled_reg(2) = '0') else ((not HEX2_data_reg(7 downto 7)) & hex_lut(to_integer(unsigned(HEX2_data_reg(3 downto 0)))));
	HEX3 <= "11111111" when (HEX_enabled_reg(3) = '0') else ((not HEX3_data_reg(7 downto 7)) & hex_lut(to_integer(unsigned(HEX3_data_reg(3 downto 0)))));
	HEX4 <= "11111111" when (HEX_enabled_reg(4) = '0') else ((not HEX4_data_reg(7 downto 7)) & hex_lut(to_integer(unsigned(HEX4_data_reg(3 downto 0)))));
	HEX5 <= "11111111" when (HEX_enabled_reg(5) = '0') else ((not HEX5_data_reg(7 downto 7)) & hex_lut(to_integer(unsigned(HEX5_data_reg(3 downto 0)))));
		
	-- Processes
	bus_register_read_process:
	process(cs_n,read_n,addr) begin
		if ((cs_n = '0') and (read_n = '0')) then
			case addr is
				when "000" =>
					dout(31 downto 6) <= (others => '0');
					dout(5 downto 0) <= HEX_enabled_reg;
				when "001" =>
					dout(31 downto 8) <= (others => '0');
					dout(7 downto 0) <= HEX0_data_reg;
				when "010" =>
					dout(31 downto 8) <= (others => '0');
					dout(7 downto 0) <= HEX1_data_reg;
				when "011" =>
					dout(31 downto 8) <= (others => '0');
					dout(7 downto 0) <= HEX2_data_reg;
				when "100" =>
					dout(31 downto 8) <= (others => '0');
					dout(7 downto 0) <= HEX3_data_reg;
				when "101" =>
					dout(31 downto 8) <= (others => '0');
					dout(7 downto 0) <= HEX4_data_reg;
				when "110" =>
					dout(31 downto 8) <= (others => '0');
					dout(7 downto 0) <= HEX5_data_reg;
				when others =>
					-- Return a 0 value at invalid address
					dout <= (others => '0');
			end case;
		else
			dout <= (others => 'X');
		end if;
	end process;

	bus_register_write_process:
	process(clk,reset_n) begin
		if (reset_n = '0') then
			HEX_enabled_reg <= (others => '0'); -- Disable all displays on reset
		elsif (rising_edge(clk)) then
			if ((cs_n = '0') and (write_n = '0')) then
				-- Write to register
				case addr is
					when "000" => HEX_enabled_reg <= din(5 downto 0);
					when "001" => HEX0_data_reg <= din(7 downto 0);
					when "010" => HEX1_data_reg <= din(7 downto 0);
					when "011" => HEX2_data_reg <= din(7 downto 0);
					when "100" => HEX3_data_reg <= din(7 downto 0);
					when "101" => HEX4_data_reg <= din(7 downto 0);
					when "110" => HEX5_data_reg <= din(7 downto 0);
					when others => null;
				end case;
			end if;
		end if;
	end process;

end;