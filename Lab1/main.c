#include "msp.h"

#define ACLK 32768
#define ADC_RANGE 16384
#define SERVO_DUTY 0.025
#define SERVO_PERIOD 20
#define ADC_IN 1
#define PWM_OUT 4
#define ADC_CONVERSION_PERIOD 100
/**
 * main.c
 */
void adjust_duty(int, float);
void init_PWM(int, float);
void init_ADC(int);
float calculate_duty(int);

uint32_t ms2ticks(float ms)
{
    return ACLK * ms / 1000;
}

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    NVIC_EnableIRQ(ADC14_IRQn);
    NVIC_EnableIRQ(TA1_0_IRQn);

    // selecting P4.0 as A13
    P4->DIR = 0x00;
    P4->SEL0 = 1 << 0;
    P4->SEL1 = 1 << 0;

    // setting P2.4 as PWM output port
    P2->DIR = 1 << PWM_OUT;
    P2->SEL0 = 1 << PWM_OUT;
    P2->SEL1 = 0 << PWM_OUT;

    init_PWM(SERVO_PERIOD, SERVO_DUTY);
    init_ADC(0);

    TIMER_A1->CTL = TIMER_A_CTL_TASSEL_1 | TIMER_A_CTL_MC_1;
    TIMER_A1->CCR[0] = ms2ticks(ADC_CONVERSION_PERIOD);
    TIMER_A1->CCTL[0] |= TIMER_A_CCTLN_CCIE;

    while(1);
}

void ADC14_IRQHandler(void)
{
    ADC14->CLRIFGR0 |= ADC14_CLRIFGR0_CLRIFG0;
    adjust_duty(SERVO_PERIOD, calculate_duty(ADC14->MEM[0]));
}

void TA1_0_IRQHandler(void)
{
    TIMER_A1->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
    ADC14->CTL0 |= ADC14_CTL0_SC;
}

float calculate_duty(int ADC_value)
{
    return SERVO_DUTY + (float)ADC_value / ADC_RANGE * 0.1;
}

void adjust_duty(int period, float duty_cycle)
{
    TIMER_A0->CCR[1] = ms2ticks(period * duty_cycle);
}

void init_PWM(int period, float duty_cycle)
{
    TIMER_A0->CTL = TIMER_A_CTL_TASSEL_1 | TIMER_A_CTL_MC_1;
    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;
    TIMER_A0->CCR[0] = ms2ticks(period);
    TIMER_A0->CCR[1] = ms2ticks(period * duty_cycle);
}

void init_ADC(int mem_addr)
{
    /* param mem_addr should be from 0 to 31 */

    // setting ADC input as A13
    ADC14->MCTL[0] = ADC14_MCTLN_INCH_13;

    // ADC14ON & select ACLK & select single channel repeat mode &
    // select sample clock as SAMPCON
    ADC14->CTL0 |= ADC14_CTL0_ON | ADC14_CTL0_SSEL_2 | ADC14_CTL0_SHP | ADC14_CTL0_CONSEQ_2;

    // select the output mem register for conversion
    ADC14->CTL1 |= mem_addr << ADC14_CTL1_CSTARTADD_OFS;

    // enable interrupt for mem[0]
    ADC14->IER0 |= ADC14_IER0_IE0;

    // enable conversion
    ADC14->CTL0 |= ADC14_CTL0_ENC;
}
