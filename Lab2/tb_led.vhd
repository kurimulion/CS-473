library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity tb_led is
end tb_led;

architecture test of tb_led is
    signal clk: std_logic := '0';
    signal nReset: std_logic := '1';

    signal address   : std_logic_vector(3 downto 0);
    signal write     : std_logic;
    signal writedata : std_logic_vector(31 downto 0);
    signal read      : std_logic;
    signal readdata  : std_logic_vector(31 downto 0);
    signal dout      : std_logic;

    constant clk_period : time := 20 ns;
begin
    dut: entity work.led_interface_auto
        port map(
            clk => clk,
            nReset => nReset,
            address => address,
            write => write,
            writedata => writedata,
	        read => read,
            readdata => readdata,
            dout => dout
        );

    clk <= not clk after 10 ns;

    simulation: process
    begin
        nReset <= '0';
        wait for 20 ns;
        nReset <= '1';
        
        address <= "0001";
        write <= '1';
        writedata <= std_logic_vector(to_unsigned(1, writedata'length));
        wait for 20 ns;

        address <= "0010";
        writedata <= std_logic_vector(to_unsigned(2, writedata'length));
        wait for 20 ns;

        address <= "0011";
        writedata <= std_logic_vector(to_unsigned(4, writedata'length));
        wait for 20 ns;

        address <= "0100";
        writedata <= std_logic_vector(to_unsigned(8, writedata'length));
        wait for 20 ns;

        address <= "0101";
        writedata <= std_logic_vector(to_unsigned(16, writedata'length));
        wait for 20 ns;

        address <= "0110";
        writedata <= std_logic_vector(to_unsigned(32, writedata'length));
        wait for 20 ns;

        address <= "0111";
        writedata <= std_logic_vector(to_unsigned(64, writedata'length));
        wait for 20 ns;

        address <= "1000";
        writedata <= std_logic_vector(to_unsigned(128, writedata'length));
        wait for 20 ns;

        address <= "0000";
        writedata <= std_logic_vector(to_unsigned(1, writedata'length));
        wait for 20 ns;

        write <= '0';
        wait;

    end process simulation;

end architecture test;
