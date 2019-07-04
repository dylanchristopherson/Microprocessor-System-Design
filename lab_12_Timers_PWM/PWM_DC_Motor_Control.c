#include "MKL25Z4.h"

void delayMs (int n);

int main(void) {
	int pw = 0;
	
	/* enable PTC 6-3 as output */
	SIM->SCGC5 |= 0x800; 			/* enable clock to Port C */
	PORTC->PCR[3] = 0x100;    /* make PTC3 pin as GPIO */
	PORTC->PCR[3] = 0x100;    /* make PTC4 pin as GPIO */
	PTC->PDDR |= 0x18;        /* make PTC4-3 as output pin */
	PTC->PSOR = 1<< 3;
	PTC->PCOR = 1 << 4;

	/* enable PTA 12 as output of TPM1_CH0 */
	SIM->SCGC5 |= 0x200;      /* enable clock to Port A */
	PORTA->PCR[12] = 0x300;   /* make PTA12 pin as TPM1 output */
	
	/* enable TPM1_CH0 */
	SIM->SCGC6 |= 0x2000000u;   /* enable clock to TPM1 */
	SIM->SOPT2 |= 0x01000000;   /* use MCGFLLCLK as time counter clock */
	TPM1->SC = 0;               /* disable timer */
	TPM1->CONTROLS[0].CnSC = 0x20 | 0x08;   /* edge-aligned, pulse high */
	TPM1->MOD = 4193;                   /* Set up module register for 10kHz */
	TPM1->CONTROLS[0].CnV = 100;       /* Startup dutycycle */
	TPM1->CONTROLS[0].CnV = 100;   /* Startup dutycycle */
	TPM1->SC = 0x08;               /* enable PTM1 */
	
	while(1) {
	
		// speed up
		for (pw = 100; pw < 4000; pw += 20) // was pw += 20
		{
			TPM1->CONTROLS[0].CnV = pw;
			delayMs(30); // was 50
		}
		
		// slow down
		for (pw = 4000; pw > 100; pw -= 20)
		{
			TPM1->CONTROLS[0].CnV = pw;
			delayMs(30);
		}
		
			
		// change direction 
		PTC->PCOR = 1 << 3;
		PTC->PSOR = 1 << 4;
		
		
		// speed up
		for (pw = 100; pw < 4000; pw += 20) // was pw += 20
		{
			TPM1->CONTROLS[0].CnV = pw;
			delayMs(30); // was 50
		}
		
		// slow down
		for (pw = 4000; pw > 100; pw-= 20)
		{
			TPM1->CONTROLS[0].CnV = pw;
			delayMs(30); // was 50
		}
		
		
		// change direction
		PTC->PSOR = 1 << 3;
		PTC->PCOR = 1 << 4;
		
		
	}
}

void delayMs(int n) {
	int i, j;
	
	for( i=0; i<n; i++)
		for(j = 0; j<3180; j++) {}  // do nothing
}