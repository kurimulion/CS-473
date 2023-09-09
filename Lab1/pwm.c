#include "msp.h"

#define ACLK 32768
/**
 * main.c
 */
void init_PWM(int, float);

uint32_t ms2ticks(float ms) {
    return 32768 * ms / 1000;
}

void main(void)
{
    float duty = 0.8;
    int period = 50; // ms

	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	init_PWM(period, duty);

	// set up P2.4 for output as Timer_A0
	P2->DIR = 1 << 4;
	P2->SEL0 = 1 << 4;
	P2->SEL1 = 0 << 4;

	while(1);
}

void init_PWM(int period, float duty_cycle)
{
	TIMER_A0->CCR[0] = ms2ticks(period);
	TIMER_A0->CCR[1] = ms2ticks(period * duty_cycle);
	TIMER_A0->CTL = TIMER_A_CTL_TASSEL_1 | TIMER_A_CTL_MC_1;
	TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;
}