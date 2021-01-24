-- Author: Linus Eriksson
-- Date: 2017-04-23

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity VGA_IP is
	generic 
	(
		DATA_WIDTH : natural := 3;
		ADDR_WIDTH : natural := 17
	);
	port(
		-- Avalon interface
		reset_n : in std_logic;
		clk 	: in std_logic;
		cs_n 	: in std_logic;
		addr 	: in std_logic_vector(ADDR_WIDTH-1 downto 0);
		write_n : in std_logic;
		read_n 	: in std_logic;
		din 	: in std_logic_vector(31 downto 0);
		dout 	: out std_logic_vector(31 downto 0);
		
		-- VGA signals
		vga_clock	: in std_logic;
		vga_r 		: out std_logic_vector(3 downto 0);
		vga_g 		: out std_logic_vector(3 downto 0);
		vga_b 		: out std_logic_vector(3 downto 0);
		vga_hs		: out std_logic;
		vga_vs		: out std_logic);
end;

architecture rtl of VGA_IP is
	signal cpu_we_n : std_logic;
	signal cpu_read_data : std_logic_vector(DATA_WIDTH-1 downto 0);
	signal cpu_write_data : std_logic_vector(DATA_WIDTH-1 downto 0);

	component VGA_Controller is
		port(
			vga_clock 		: in std_logic;
			cpu_clock 		: in std_logic;
			reset_n 		: in std_logic;
			cpu_address 	: in std_logic_vector(ADDR_WIDTH-1 downto 0);
			cpu_we_n 		: in std_logic;
			cpu_data_in 	: in std_logic_vector(DATA_WIDTH-1 downto 0);
			cpu_data_out 	: out std_logic_vector(DATA_WIDTH-1 downto 0);
			vga_r 			: out std_logic_vector(3 downto 0);
			vga_g 			: out std_logic_vector(3 downto 0);
			vga_b 			: out std_logic_vector(3 downto 0);
			vga_hs 			: out std_logic;
			vga_vs 			: out std_logic);
	end component;
begin
	
	VGA_Controller_inst : VGA_Controller
	port map(
		vga_clock => vga_clock,
		cpu_clock => clk,
		reset_n => reset_n,
		cpu_address => addr,
		cpu_we_n => cpu_we_n,
		cpu_data_in => cpu_write_data,
		cpu_data_out => cpu_read_data,
		vga_r => vga_r,
		vga_g => vga_g,
		vga_b => vga_b,
		vga_hs => vga_hs,
		vga_vs => vga_vs);
	
	-- CPU reads from component
	bus_register_read_process:
	process(cs_n,read_n,addr) begin
		if ((cs_n = '0') and (read_n = '0')) then
			dout(31 downto DATA_WIDTH) <= (others => '0');
			dout(DATA_WIDTH-1 downto 0) <= cpu_read_data;
		else 
			dout <= (others => 'X');
		end if;
	end process;
	
	-- CPU writes to component
	bus_register_write_process:
	process(clk,reset_n) begin
		if (reset_n = '0') then
			cpu_we_n <= '1';
		elsif (rising_edge(clk)) then
			if ((cs_n = '0') and (write_n = '0')) then
				cpu_we_n <= '0';
				cpu_write_data <= din(DATA_WIDTH-1 downto 0);
			else
				cpu_we_n <= '1';
			end if;
		end if;
	end process;
	
	
end;
