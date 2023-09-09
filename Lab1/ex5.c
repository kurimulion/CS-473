#include "msp.h"

#define ACLK 32768
/**
 * main.c
 */
uint32_t ms2ticks(float ms)
{
    return ACLK * ms / 1000;
}

void main(void)
{
    int period = 50; // ms

	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	NVIC_EnableIRQ(TA0_0_IRQn);
	NVIC_SetPriority(TA0_0_IRQn, 4);
	TIMER_A0->CTL = TIMER_A_CTL_TASSEL_1 | TIMER_A_CTL_MC_1;
	TIMER_A0->CCR[0] = ms2ticks(period);
	TIMER_A0->CCTL[0] |= TIMER_A_CCTLN_CCIE;

	P2->DIR = 0xFF;
	P2->OUT = 1 << 7;

	while(1) {
	}
}

void TA0_0_IRQHandler(void)
{
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
    P2->OUT ^= 1 << 7;
}
