library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity led_interface_auto is
    port(
        clk    : in std_logic;
        nReset : in std_logic;

        address   : in std_logic_vector(3 downto 0);
        write     : in std_logic;
        writedata : in std_logic_vector(31 downto 0);
        read      : in std_logic;
        readdata  : out std_logic_vector(31 downto 0);

        -- External interface
        dout : out std_logic
    );
end entity led_interface_auto;

architecture rtl of led_interface_auto is

    type std_logic_data is array (natural range <>) of std_logic_vector(23 downto 0);

    -- registers
    -- should be generic?
    signal RegData  : std_logic_data(0 to 7);
    signal RegStart : std_logic_vector(7 downto 0);

    signal state        : std_logic_vector(2 downto 0);
    signal data_pointer : std_logic_vector(2 downto 0);
    signal bit_pointer  : std_logic_vector(4 downto 0);
    signal counter      : std_logic_vector(11 downto 0);

begin
    
    process(clk, nReset, state)
    begin
        if nReset = '0' then
            state <= "000";
        elsif rising_edge(clk) then
            case state is
                -- reset state
                when "000" =>
		            dout <= '0';
                    if RegStart(0) = '1' then
                        state <= "001";
                        data_pointer <= std_logic_vector(to_unsigned(0, data_pointer'length));
                        bit_pointer <= std_logic_vector(to_unsigned(23, bit_pointer'length));
                    end if;
                when "001" =>
                    if RegData(to_integer(unsigned(data_pointer)))(to_integer(unsigned(bit_pointer))) = '0' then
                        state <= "010";
                    else
                        state <= "011";
                    end if;
                    counter <= (others => '0');
                -- 0 high
                when "010" =>
                    dout <= '1';
		            counter <= std_logic_vector(unsigned(counter) + 1);
                    if counter = std_logic_vector(to_unsigned(17, counter'length)) then
                        state <= "100";
                        counter <= (others => '0');
                    end if;
                -- 1 high
                when "011" =>
                    dout <= '1';
		            counter <= std_logic_vector(unsigned(counter) + 1);
                    if counter = std_logic_vector(to_unsigned(35, counter'length)) then
                        state <= "101";
                        counter <= (others => '0');
                    end if;
                -- 0 low
                when "100" =>
                    dout <= '0';
		            counter <= std_logic_vector(unsigned(counter) + 1);
                    if counter = std_logic_vector(to_unsigned(40, counter'length)) then
                        state <= "110";
                        counter <= (others => '0');
                    end if;
                -- 1 low
                when "101" =>
                    dout <= '0';
		            counter <= std_logic_vector(unsigned(counter) + 1);
                    if counter = std_logic_vector(to_unsigned(30, counter'length)) then
                        state <= "110";
                        counter <= (others => '0');
                    end if;
                -- read next bit
                when "110" =>
                    if bit_pointer = std_logic_vector(to_unsigned(0, bit_pointer'length)) then
                        if data_pointer = std_logic_vector(to_unsigned(7, data_pointer'length)) then
                            state <= "111";
                        else
                            bit_pointer <= std_logic_vector(to_unsigned(23, bit_pointer'length));
                            data_pointer <= std_logic_vector(unsigned(data_pointer) + 1);
                            state <= "001";
                        end if;
                    else
                        bit_pointer <= std_logic_vector(unsigned(bit_pointer) - 1);
                        state <= "001";
                    end if;
                -- data refresh cycle finished
                when "111" =>
                    dout <= '0';
                    counter <= std_logic_vector(unsigned(counter) + 1);
                    if counter = std_logic_vector(to_unsigned(2500, counter'length)) then
                        state <= "000";
                    end if;
                when others => state <= "000";
            end case;
        end if;
    end process;

    process(clk, nReset)
    begin
        if nReset = '0' then
            for i in 0 to 7 loop
                RegData(i) <= (others => '0');
            end loop;
	        RegStart <= (others => '0');
        elsif rising_edge(clk) then
            if write = '1' then
                case address is
                    when "0000" => RegStart <= writedata(7 downto 0);
                    when "0001" => RegData(0) <= writedata(23 downto 0);
                    when "0010" => RegData(1) <= writedata(23 downto 0);
                    when "0011" => RegData(2) <= writedata(23 downto 0);
                    when "0100" => RegData(3) <= writedata(23 downto 0);
                    when "0101" => RegData(4) <= writedata(23 downto 0);
                    when "0110" => RegData(5) <= writedata(23 downto 0);
                    when "0111" => RegData(6) <= writedata(23 downto 0);
                    when "1000" => RegData(7) <= writedata(23 downto 0);
                    when others => null;
                end case;
            end if;
        end if;
    end process;

    process(clk)
    begin
        if rising_edge(clk) then
            readdata <= (others => '0');
            if read = '1' then
                case address is
                    when "0000" => readdata(7 downto 0) <= RegStart;
                    when "0001" => readdata(23 downto 0) <= RegData(0);
                    when "0010" => readdata(23 downto 0) <= RegData(1);
                    when "0011" => readdata(23 downto 0) <= RegData(2);
                    when "0100" => readdata(23 downto 0) <= RegData(3);
                    when "0101" => readdata(23 downto 0) <= RegData(4);
                    when "0110" => readdata(23 downto 0) <= RegData(5);
                    when "0111" => readdata(23 downto 0) <= RegData(6);
                    when "1000" => readdata(23 downto 0) <= RegData(7);
                    when others => null;
                end case;
            end if;
        end if;
    end process;

end;

