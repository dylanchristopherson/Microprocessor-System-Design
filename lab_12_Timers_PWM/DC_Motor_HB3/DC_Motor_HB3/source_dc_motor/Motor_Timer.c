/*
 * This program controls the rotational speed and direction of 
 * a DC motor.
 *
 * The Digilent HB3 board has a half-H-bridge motor driver
 * which is capable of driving a DC motor
 
 * The pulse period is set by the MOD register. Using the CnV
 * register we set the pulse width (duty cycle). If CnV =0, then
 * output has 0 duty cycle. If CnV is greater than or equal to
 * MOD, the duty cycle is 100%.

 * Duty Cycle = CnV/(MOD+1) * 100%
 * The DC motor will be running from a slowest speed (2% Duty Cycle)
 * to the max speed (100% Duty Cycle) at 10KHz with a duty cycle varying
 * from 2% to 100%.
 
 * We are going to create a duty cycle from 2% to 100%
 * so the starting CnV register value is 100 and increase the value 
 * each time by 60 in a for loop and reach the final CnV value 4000.
 
 * Calculate TMPx_MOD value = (41.98MHz / 10KHzHz)-1 = 4,197. 
 
 * This program uses PORTC 3 or 4 for direction control and PTA 12 for
 * PWM control.  
 * The PWM is generated by TPMx_CH0.
 */

#include "MKL25Z4.h"

void delayMs(int n);

int main(void)
{
    int pw = 0;

    /* enable PTC 6-3 as output */
    SIM->SCGC5 |= 0x800;        /* enable clock to Port C */
    PORTC->PCR[3] = 0x100;      /* make PTC3 pin as GPIO */
    PORTC->PCR[4] = 0x100;      /* make PTC4 pin as GPIO */
    PTC->PDDR |= 0x18;          /* make PTC4-3 as output pin */
    PTC->PSOR = 1 << 3;
    PTC->PCOR = 1 << 4;
    
    /* enable PTA 12 as output of TPM1_CH0 */
    SIM->SCGC5 |= 0x200;        /* enable clock to Port A */
    PORTA->PCR[12] = 0x300;     /* make PTA12 pin as TPM1 output */

    /* enable TPM1_CH0 */
    SIM->SCGC6 |= 0x2000000u;   /* enable clock to TPM1 */
    SIM->SOPT2 |= 0x01000000;   /* use MCGFLLCLK as timer counter clock */
    TPM1->SC = 0;               /* disable timer */
    TPM1->CONTROLS[0].CnSC = 0x20 | 0x08;   /* edge-aligned, pulse high */
    TPM1->MOD = 4193;           /* Set up modulo register for 10kHz */
    TPM1->CONTROLS[0].CnV = 100;  /* Startup dutycycle */
    TPM1->SC = 0x08;            /* enable TPM1 */
    
    while(1)
    {
        // speed up
        for (pw = 100; pw < 4000; pw += 60) //was pw +=20
        {
            TPM1->CONTROLS[0].CnV = pw;
            delayMs(10); //was 50
        }
        // slow down
        for (pw = 4000; pw >100; pw -= 20)
        {
            TPM1->CONTROLS[0].CnV = pw;
            delayMs(10);//was 50
        }
        // change dirction
        PTC->PCOR = 1 << 3;
        PTC->PSOR = 1 << 4;
        
        // speed up
        for (pw = 100; pw < 4000; pw += 60) //was pw +=20
        {
            TPM1->CONTROLS[0].CnV = pw;
            delayMs(10); //was 50
        }
        // slow down
        for (pw = 4000; pw >100; pw -= 20)
        {
            TPM1->CONTROLS[0].CnV = pw;
            delayMs(10); //was 50
        }
        // change dirction
        PTC->PSOR = 1 << 3;
        PTC->PCOR = 1 << 4;
    }
}

// delay n milliseconds (41.94MHz CPU clock)
void delayMs(int n)
{
    int i, j;
    for(i = 0 ; i < n; i++)
        for(j = 0; j < 3180; j++)
            {}  // do nothing
}

