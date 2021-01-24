-- Author: Linus Eriksson
-- Date: 2017-04-23


library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity VGA_VRAM is
	generic 
	(
		DATA_WIDTH : natural := 3;
		ADDR_WIDTH : natural := 17
	);
	port(
		-- VGA controller will only read from this RAM
		vga_clock : in std_logic;
		vga_address : in std_logic_vector(ADDR_WIDTH-1 downto 0);
		vga_data_out : out std_logic_vector(DATA_WIDTH-1 downto 0);
		
		-- CPU will read and write
		cpu_clock : in std_logic;
		cpu_address : in std_logic_vector(ADDR_WIDTH-1 downto 0);
		cpu_data_in : in std_logic_vector(DATA_WIDTH-1 downto 0);
		cpu_we_n : in std_logic := '0';
		cpu_data_out : out std_logic_vector(DATA_WIDTH-1 downto 0));
end;

architecture rtl of VGA_VRAM is
	-- Data for 3 colors channels with 4 bits each
	subtype word_t is std_logic_vector(DATA_WIDTH-1 downto 0);
	
	-- 320*240 pixels
	type memory_t is array(76799 downto 0) of word_t;
	
	shared variable ram : memory_t;
	
begin

	process(vga_clock)
	begin
		if (rising_edge(VGA_clock)) then
			vga_data_out <= ram(to_integer(unsigned(vga_address)));
		end if;
	end process;

	process(cpu_clock)
	begin
		if (rising_edge(cpu_clock)) then
			if (cpu_we_n = '0') then
				ram(to_integer(unsigned(cpu_address))) := cpu_data_in;
			end if;			
			cpu_data_out <= ram(to_integer(unsigned(cpu_address)));
		end if;
	end process;

end;
