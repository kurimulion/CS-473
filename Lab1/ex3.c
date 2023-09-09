#include "msp.h"

/**
 * main.c
 */
void delay(unsigned int);

void main(void)
{
    int period = 20; // ms

	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

    // using LFXTCLK
    TIMER_A0->CTL = TIMER_A_CTL_TASSEL_1 | TIMER_A_CTL_MC_1;

	P2->DIR = 0xFF;
	P2->OUT = (1<<7);

	while(1) {
	    delay(period/2);
        P2->OUT ^= 1 << 7;
	}
}

void delay(uint32_t ms)
{
    TIMER_A0->CCR[0] = 32768.0 * ms / 1000;

    while(ms != 0)
    {
        if(TIMER_A0->CCTL[0] & TIMER_A_CCTLN_CCIFG)
        {
            ms--;
            TIMER_A0->CCTL[0] &= (0<<TIMER_A_CCTLN_CCIFG_OFS);
        }
    }
}
