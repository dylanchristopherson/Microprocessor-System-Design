#include <MKL25Z4.H>
void delayMs(int n);

int main (void) {
	int pulseWidth = 0;
	
	SIM->SCGC5 |= 0x1000;
	PORTD->PCR[1] = 0x0400;

	SIM->SCGC6 |= 0x01000000;
	SIM->SOPT2 |= 0x01000000;
	TPM0->SC = 0;
	TPM0->CONTROLS[1].CnSC = 0x20 | 0x08;
	TPM0->MOD = 43702;
	TPM0->CONTROLS[1].CnV = 14568;
	TPM0->SC = 0x0C;
	
	while (1) {
		pulseWidth += 437;
		if (pulseWidth > 43702)
			pulseWidth = 0;
		TPM0->CONTROLS[1].CnV = pulseWidth;
		delayMs(20);
	}
}

void delayMs(int n) {
	int i, j;
	
	for( i=0; i<n; i++)
		for(j = 0; j<7000; j++) {}
}