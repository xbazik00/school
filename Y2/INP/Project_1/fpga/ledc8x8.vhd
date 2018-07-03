library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

entity ledc8x8 is
port (
	SMCLK: in std_logic;
	RESET: in std_logic;
	ROW: out std_logic_vector (7 downto 0);
	LED: out std_logic_vector (7 downto 0)
);
end ledc8x8;

architecture main of ledc8x8 is
	signal row_s: std_logic_vector (7 downto 0) := "00000001";
	signal ce: std_logic;
	signal ce_cnt: std_logic_vector (22 downto 0);
	signal change: std_logic := '0';
	signal first_letter: std_logic_vector (0 to 7);
	signal second_letter: std_logic_vector (0 to 7);
    -- Sem doplnte definice vnitrnich signalu.

begin

ce_counter: process(SMCLK)
begin
	if RESET = '1' then
		ce_cnt <= (others => '0');
	elsif SMCLK'event and SMCLK = '1' then
		ce_cnt <= ce_cnt + 1;
		if ce_cnt (7 downto 0) = "11111111" then
			ce <= '1';	      
		else
		       	ce <= '0';
		end if;
		if ce_cnt (22 downto 0) = "11100001000000000000000" then 
			if change = '0' then
				change <= '1';
			else
				change <= '0';
			end if;
		end if;
	end if;
end process ce_counter;

activate_row: process(RESET, SMCLK)
begin
	if RESET = '1' then
		row_s <= "00000001";
	elsif SMCLK'event and SMCLK = '1' then
		if ce = '1' then
			row_s <= row_s(0)  & row_s(7 downto 1);
		end if;
	end if;
end process;

map_first_letter: process(row_s)
begin
	case row_s is
		when "00000001" => first_letter <= "01111110";
		when "00000010" => first_letter <= "00111100";
		when "00000100" => first_letter <= "01011010";
		when "00001000" => first_letter <= "01100110";
		when "00010000" => first_letter <= "01111110";
		when "00100000" => first_letter <= "01111110";
		when "01000000" => first_letter <= "01111110";
		when "10000000" => first_letter <= "01111110";
		when others => null; 
	end case;
end process;

map_second_letter: process(row_s)
begin
	case row_s is
		when "00000001" => second_letter <= "11100000";
		when "00000010" => second_letter <= "11011110";
		when "00000100" => second_letter <= "11011110";
		when "00001000" => second_letter <= "11100000";
		when "00010000" => second_letter <= "11011110";
		when "00100000" => second_letter <= "10111110";
		when "01000000" => second_letter <= "10111110";
		when "10000000" => second_letter <= "11000000";
		when others => null; 
	end case;
end process;

display: process(row_s)
begin
	ROW <= row_s;
	case change is
		when '0' => LED <= first_letter;
		when '1' => LED <= second_letter;
		when others => null;
	end case;
end process;

end main;
