#include "msp.h"

/**
 * main.c
 */
void init_ADC(int);

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	NVIC_EnableIRQ(ADC14_IRQn);

	// selecting P4.0 as A13
	P4->DIR = 0x00;
	P4->SEL0 = 1 << 0;
	P4->SEL1 = 1 << 0;

	init_ADC(0);

	while(1);
}

void ADC14_IRQHandler(void)
{
    ADC14->CLRIFGR0 |= ADC14_CLRIFGR0_CLRIFG0;
    ADC14->CTL0 |= ADC14_CTL0_SC;
}

void setup_ADC(int mem_addr)
{
	/* param mem_addr should be from 0 to 31 */
	// setting ADC input as A13
	ADC14->MCTL[0] = ADC14_MCTLN_INCH_13;

	// ADC14ON & select ACLK & select single channel repeat mode &
	// select sample clock as SAMPCON
	ADC14->CTL0 |= ADC14_CTL0_ON | ADC14_CTL0_SSEL_2 | ADC14_CTL0_SHP | ADC14_CTL0_CONSEQ_2;

	// select the output mem register for conversion
	ADC14->CTL1 |= mem_addr<<ADC14_CTL1_CSTARTADD_OFS;

	// enable interrupt for mem[0]
	ADC14->IER0 |= ADC14_IER0_IE0;

	// enable conversion
	ADC14->CTL0 |= ADC14_CTL0_ENC;

	// start conversion
	ADC14->CTL0 |= ADC14_CTL0_SC;
}