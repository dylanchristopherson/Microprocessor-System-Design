/* 
* This program drives a standard servo motor
*
* The TPM1_CH0 is used in PWM mode to generate a 50 Hz signal
* with pulse width between 1ms and 2 ms
* The H-bridge motor driver (Digilent CON 3 board) is used
*
*/

#include "MKL25Z4.h"

void delayMs(int n);

int main(void)
{
	int pw = 0;
	
	/* enable PTA 12 as output of TPM1_CH0 */
	SIM->SCGC5 |= 0x200;     /* enable clock to Port A */
	PORTA->PCR[12] = 0x300;  /* make PTA12 pin as TPM1 output */
	
	/* enable TPM1_CH0 */
	SIM->SCGC6 |= 0x2000000u; /* enalbe clock to TPM1 */
	SIM->SOPT2 |= 0x01000000; /* use MCGFLLCLK as time counter clock */
	TPM1->SC = 0;             /* reset timer */
	TPM1->CONTROLS[0].CnSC = 0x20 | 0x08;
	TPM1->MOD = 52404;
	TPM1->CONTROLS[0].CnV = 100;
	TPM1->SC = 0x0C;
	
	while(1) {
		for (pw = 2620 - 1; pw < 5240 - 1; pw += 100)
		{
			TPM1->CONTROLS[0].CnV = pw;
			delayMs(100);
		}
	}
}

void delayMs(int n) {
	int i, j;
	
	for( i=0; i<n; i++)
		for(j = 0; j<7000; j++) {}  // do nothing
}