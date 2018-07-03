-- cpu.vhd: Simple 8-bit CPU (BrainLove interpreter)
-- Copyright (C) 2016 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): DOPLNIT
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet ROM
   CODE_ADDR : out std_logic_vector(11 downto 0); -- adresa do pameti
   CODE_DATA : in std_logic_vector(7 downto 0);   -- CODE_DATA <- rom[CODE_ADDR] pokud CODE_EN='1'
   CODE_EN   : out std_logic;                     -- povoleni cinnosti
   
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(9 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni (1) / zapis (0)
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna
   IN_REQ    : out std_logic;                     -- pozadavek na vstup data
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- LCD je zaneprazdnen (1), nelze zapisovat
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

	type instr_type is (inc_p,dec_p,inc_v,dec_v,while_s,while_e,print,read,store,load,halt,nop);

	signal instr:instr_type;
	
	signal PC_inc:std_logic;
	signal PC_dec:std_logic;
	signal PC_addr:std_logic_vector(11 downto 0);

	signal PTR_inc:std_logic;
	signal PTR_dec:std_logic;
	signal PTR_addr:std_logic_vector(9 downto 0);

	signal CNT_inc:std_logic;
	signal CNT_dec:std_logic;
	signal CNT_one:std_logic;
	signal CNT_addr:std_logic_vector(7 downto 0);

	signal TMP_ld:std_logic;
	signal TMP_data:std_logic_vector(7 downto 0);

	signal sel:std_logic_vector(1 downto 0);

	
	type FSM_state is (sidle,sfetch,sdecode,sinc_p,sdec_p,sinc_v,sinc_v2,sdec_v,sdec_v2,swhile_s1,swhile_s2,swhile_s3,swhile_s4,swhile_s5,swhile_e1,swhile_e2,swhile_e3,swhile_e32,swhile_e4,swhile_e5,sprint,sprint2,sread,sread2,sstore,sstore2,sstore3,sload,sload2,shalt,snop);
	signal pstate:FSM_state;
	signal nstate:FSM_state;

	
 -- zde dopiste potrebne deklarace signalu

begin
	-- ----------------------------------------------------------------------------
	--                      PC register
	-- ----------------------------------------------------------------------------
	pc_reg: process(CLK,RESET) begin
		if(RESET = '1') then
			PC_addr <= (others => '0');
		elsif(CLK'event) and (CLK = '1') then
			if(PC_inc = '1') then
				PC_addr <= PC_addr + 1;
			elsif(PC_dec = '1') then
				PC_addr <= PC_addr - 1;
			end if;
		end if;
	end process;

	CODE_ADDR <= PC_addr;

	-- ----------------------------------------------------------------------------
	--                      PTR register
	-- ----------------------------------------------------------------------------
	ptr_reg: process(CLK,RESET) begin
		if(RESET = '1') then
			PTR_addr <= (others => '0');
		elsif(CLK'event) and (CLK = '1') then
			if(PTR_inc = '1') then
				PTR_addr <= PTR_addr + 1;
			elsif(PTR_dec = '1') then
				PTR_addr <= PTR_addr - 1;
			end if;
		end if;
	end process;

	DATA_ADDR <= PTR_addr;

	-- ----------------------------------------------------------------------------
	--                      CNT register
	-- ----------------------------------------------------------------------------
	cnt_reg: process(CLK,RESET) begin
		if(RESET = '1') then
			CNT_addr <= (others => '0');
		elsif(CLK'event) and (CLK = '1') then
			if(CNT_one = '1') then
				CNT_addr <= "00000001";
			else
				if(CNT_inc = '1') then
					CNT_addr <= CNT_addr + 1;
				elsif(CNT_dec = '1') then
					CNT_addr <= CNT_addr - 1;
				end if;
			end if;
		end if;
	end process;
	
	-- ----------------------------------------------------------------------------
	--                      TMP register
	-- ----------------------------------------------------------------------------
	tmp_reg: process(CLK,RESET) begin
		if(RESET = '1') then
			TMP_data <= (others => '0');
		elsif(CLK'event) and (CLK = '1') then
			if(TMP_ld = '1') then
				TMP_data <= DATA_RDATA;
			end if;
		end if;
	end process;

	-- ----------------------------------------------------------------------------
	--                      INST decoder
	-- ----------------------------------------------------------------------------
	inst_dec: process(CODE_DATA) begin
		case CODE_DATA is
			when X"3E" => instr <= inc_p;
			when X"3C" => instr <= dec_p;
			when X"2B" => instr <= inc_v;
			when X"2D" => instr <= dec_v;
			when X"5B" => instr <= while_s;
			when X"5D" => instr <= while_e;
			when X"2E" => instr <= print;
			when X"2C" => instr <= read;
			when X"24" => instr <= load;
			when X"21" => instr <= store;
			when X"00" => instr <= halt;
			when others => instr <= nop;
		end case;
	end process;

	-- ----------------------------------------------------------------------------
	--                      MX
	-- ----------------------------------------------------------------------------
	multiplexor: process(CLK,TMP_data,sel) begin
		case sel is
			when "00" => DATA_WDATA <= IN_DATA;
			when "01" => DATA_WDATA <= TMP_data;
			when "10" => DATA_WDATA <= DATA_RDATA - 1;
			when "11" => DATA_WDATA <= DATA_RDATA + 1;
			when others =>
		end case;
	end process;

	-- ----------------------------------------------------------------------------
	--                      FSM state
	-- ----------------------------------------------------------------------------
	fsm_pstate: process(CLK,RESET) begin
		if(RESET = '1') then 
			pstate <= sidle;
		elsif (CLK'event) and (CLK = '1') then
			if(EN = '1')then
				pstate <= nstate;
			end if;
		end if;
	end process;

	-- ----------------------------------------------------------------------------
	--                      FSM next state automat
	-- ----------------------------------------------------------------------------
	fsm_nstate: process(pstate,IN_VLD,IN_DATA,sel,CNT_addr,DATA_RDATA,OUT_BUSY,instr)
	begin
	DATA_EN <= '0';
	DATA_RDWR <= '0';
	IN_REQ <= '0';
	OUT_WE <= '0';
	CODE_EN <= '0';

	PC_inc <= '0';
	PC_dec <= '0';

	PTR_inc <= '0';
	PTR_dec <= '0';

	CNT_inc <= '0';
	CNT_dec <= '0';
	CNT_one <= '0';

	TMP_ld <= '0';
	sel <= "00";

	case pstate is
	-- -----------------------------idle--------------------------------------------
			when sidle => nstate <= sfetch;

	-- -----------------------------fetch-------------------------------------------
			when sfetch => 
				CODE_EN <= '1';
				nstate <= sdecode;

	-- -----------------------------decode------------------------------------------
			when sdecode => 
				case instr is
					when inc_p => nstate <= sinc_p;
					when dec_p => nstate <= sdec_p;
					when inc_v => nstate <= sinc_v;
					when dec_v => nstate <= sdec_v;
					when while_s => nstate <= swhile_s1;
					when while_e => nstate <= swhile_e1;
					when print => nstate <= sprint;
					when read => nstate <= sread;
					when store => nstate <= sstore;
					when load => nstate <= sload;
					when halt => nstate <= shalt;
					when others => nstate <= snop;
				end case;

	-- -----------------------------increase-pointer---------------------------------
			when sinc_p =>
				PC_inc <= '1';
				PTR_inc <= '1';
				nstate <= sfetch;

	-- -----------------------------decrease-pointer---------------------------------
			when sdec_p =>
				PC_inc <= '1';
				PTR_dec <= '1';
				nstate <= sfetch;

	-- -----------------------------increase-value-----------------------------------
			when sinc_v =>
				DATA_EN <= '1';
				DATA_RDWR <= '1';
				nstate <= sinc_v2;

			when sinc_v2 =>
				DATA_EN <= '1';
				DATA_RDWR <= '0';
				PC_inc <= '1';
				sel <= "11";
				nstate <= sfetch;

	-- -----------------------------decrease-value-----------------------------------
			when sdec_v =>
				DATA_EN <= '1';
				DATA_RDWR <= '1';
				nstate <= sdec_v2;

			when sdec_v2 =>
				DATA_EN <= '1';
				DATA_RDWR <= '0';
				PC_inc <= '1';
				sel <= "10";
				nstate <= sfetch;

	-- -----------------------------print--------------------------------------------
			when sprint =>
				DATA_EN <= '1';
				DATA_RDWR <= '1';
				nstate <= sprint2;

			when sprint2 =>
				if(OUT_BUSY = '0') then
					OUT_WE <= '1';
					OUT_DATA <= DATA_RDATA;
					PC_inc <= '1';
					nstate <= sfetch;
				else nstate <= sprint2;
				end if;

	-- -----------------------------read---------------------------------------------
			when sread =>
				IN_REQ <= '1';
				nstate <= sread2;

			when sread2 =>
				IN_REQ <= '1';
				nstate <= sread2;
				if(IN_VLD = '1') then
					DATA_EN <= '1';
					DATA_RDWR <= '0';
					sel <= "00";
					PC_inc <= '1';
					nstate <= sfetch;
				end if;

	-- -----------------------------while-start--------------------------------------
			when swhile_s1 =>
				PC_inc <= '1';
				DATA_EN <= '1';
				DATA_RDWR <= '1';
				nstate <= swhile_s2;

			when swhile_s2 =>
				if(DATA_RDATA = "00000000") then
					CNT_one <= '1';
					nstate <= swhile_s3;
				else nstate <= sfetch;
				end if;

			when swhile_s3 =>
				CODE_EN <= '1';
				nstate <= swhile_s4;


			when swhile_s4 =>
				if(CODE_DATA = X"5B") then
					CNT_inc <= '1';
				elsif(CODE_DATA = X"5D") then
					CNT_dec <= '1';
				end if;
				PC_inc <= '1';
				nstate <= swhile_s5;
		
			when swhile_s5 =>
				if(CNT_addr = "00000000") then
					nstate <= sfetch;
				else
					nstate <= swhile_s3;
				end if;


	-- -----------------------------while-end----------------------------------------
			when swhile_e1 =>
				DATA_EN <= '1';
				DATA_RDWR <= '1';
				nstate <= swhile_e2;

			when swhile_e2 =>
				if(DATA_RDATA = "00000000") then
					PC_inc <= '1';
					nstate <= sfetch;
				else 
					CNT_one <= '1';
					PC_dec <= '1';
					nstate <= swhile_e3;
				end if;

			when swhile_e3 =>
				CODE_EN <= '1';
				nstate <= swhile_e32;

			when swhile_e32 => nstate <= swhile_e4;	
			when swhile_e4 =>
				if(CNT_addr = "00000000") then
					nstate <= sfetch;
				else 
					if(CODE_DATA = X"5D") then
						CNT_inc <= '1';
					elsif(CODE_DATA = X"5B") then
						CNT_dec <= '1';
					end if;
					nstate <= swhile_e5;
				end if;
			when swhile_e5 =>
				if(CNT_addr = "00000000") then
					PC_inc <= '1';
				else
					PC_dec <= '1';
				end if;
				nstate <= swhile_e3;

	-- -----------------------------load--------------------------------------------
			when sload =>
				DATA_EN <= '1';
				DATA_RDWR <= '1';
				nstate <= sload2;

			when sload2 =>
				TMP_ld <= '1';
				PC_inc <= '1';
				nstate <= sfetch;
	
	-- -----------------------------store-------------------------------------------
			when sstore =>
				sel <= "01";
				nstate <= sstore2;

			when sstore2 =>
				sel <= "01";
				DATA_EN <= '1';
				DATA_RDWR <= '0';
				nstate <= sstore3;

			when sstore3 =>
				PC_inc <= '1';
				nstate <= sfetch;
	
	-- -----------------------------halt--------------------------------------------
			when shalt => nstate <= shalt;

	-- -----------------------------nop---------------------------------------------
			when snop => 
				nstate <= sfetch;
				PC_inc <= '1';

	-- -----------------------------else--------------------------------------------
			when others =>
		end case;
	end process;

end behavioral;
 
