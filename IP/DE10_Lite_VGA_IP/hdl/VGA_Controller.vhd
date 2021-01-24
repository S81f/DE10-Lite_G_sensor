-- Author: Linus Eriksson
-- Date: 2017-04-23

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity VGA_Controller is
	generic 
	(
		DATA_WIDTH : natural := 3;--adjust line 98-100
		ADDR_WIDTH : natural := 17
	);
	port(
		vga_clock 		: in std_logic;
		cpu_clock 		: in std_logic;
		reset_n 			: in std_logic;
		cpu_address 	: in std_logic_vector(ADDR_WIDTH-1 downto 0);
		cpu_we_n 		: in std_logic;
		cpu_data_in 	: in std_logic_vector(DATA_WIDTH-1 downto 0);
		cpu_data_out 	: out std_logic_vector(DATA_WIDTH-1 downto 0);
		vga_r 			: out std_logic_vector(3 downto 0);
		vga_g 			: out std_logic_vector(3 downto 0);
		vga_b 			: out std_logic_vector(3 downto 0);
		vga_hs 			: out std_logic;
		vga_vs 			: out std_logic);
end;

architecture rtl of VGA_Controller is
	component VGA_VRAM is
		port(
			vga_clock : in std_logic;
			vga_address : in std_logic_vector(ADDR_WIDTH-1 downto 0);
			vga_data_out : out std_logic_vector(DATA_WIDTH-1 downto 0);
			cpu_clock : in std_logic;
			cpu_address : in std_logic_vector(ADDR_WIDTH-1 downto 0);
			cpu_data_in : in std_logic_vector(DATA_WIDTH-1 downto 0);
			cpu_we_n : in std_logic;
			cpu_data_out : out std_logic_vector(DATA_WIDTH-1 downto 0));
	end component;

	signal x_counter : integer range 0 to 799 := 0;
	signal y_counter : integer range 0 to 524 := 0;	
	
	constant h_front_porch 	: integer := 16;
	constant h_display 		: integer := 640;
	constant h_sync 			: integer := 96;
	constant v_display 		: integer := 480;
	constant v_front_porch 	: integer := 10;
	constant v_sync 			: integer := 2;
	
	-- Specifies how many cycles it takes for data to arrive on rgb_color
	constant read_delay : integer := 1;
	
	-- VRAM communication wires
	signal vga_address : std_logic_vector(ADDR_WIDTH-1 downto 0);
	signal vga_data : std_logic_vector(DATA_WIDTH-1 downto 0);
	
begin
	-- VGA_VRAM
	VGA_VRAM_inst : VGA_VRAM
	port map(
		vga_clock 		=> vga_clock,
		vga_address 	=> vga_address,
		vga_data_out	=> vga_data,
		cpu_clock 		=> cpu_clock,
		cpu_address 	=> cpu_address,
		cpu_data_in 	=> cpu_data_in,
		cpu_we_n 		=> cpu_we_n,
		cpu_data_out 	=> cpu_data_out);
	
	-- Counters
	process(vga_clock,reset_n) begin
		if (reset_n = '0') then
			x_counter <= 0;
			y_counter <= 0;
		elsif (rising_edge(vga_clock)) then
			if (x_counter = 799) then
				x_counter <= 0;
				if (y_counter = 524) then
					y_counter <= 0;
				else
					y_counter <= y_counter + 1;
				end if;
			else
				x_counter <= x_counter + 1;
			end if;
		end if;
	end process;
	
	-- This VGA controller outputs image at 640x480, but uses a 320x240 RAM. 
	-- Each pixel is used 4 times in order to cover entire monitor.
	vga_address <= std_logic_vector(to_unsigned((320 * (y_counter/2)) + (x_counter/2),17)) when (y_counter < v_display) and (x_counter < h_display) else (others => '0');
	
	vga_hs <= '0' when (x_counter >= ((h_display + h_front_porch)) + read_delay) and (x_counter < ((h_display + h_front_porch + h_sync) + read_delay)) else '1';
	vga_vs <= '0' when (y_counter >= ((v_display + v_front_porch))) and (y_counter < ((v_display + v_front_porch + v_sync))) else '1';
	
	vga_r <= (others => '1') when (vga_data(2)='1') AND (x_counter >= read_delay	) and (x_counter < h_display + read_delay) and (y_counter < v_display) else (others => '0');
	vga_g <= (others => '1') when (vga_data(1)='1') AND (x_counter >= read_delay	) and (x_counter < h_display + read_delay) and (y_counter < v_display) else (others => '0');
	vga_b <= (others => '1') when (vga_data(0)='1') AND (x_counter >= read_delay	) and (x_counter < h_display + read_delay) and (y_counter < v_display) else (others => '0');
end;
