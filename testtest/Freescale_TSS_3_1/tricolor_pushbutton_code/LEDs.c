#include "MKL25Z4.H"
#include "LEDs.h"
#include "gpio_defs.h"

void Init_RGB_LEDs(void) {
	// Enable clock to ports B and D
	 SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;;
	
	// Make 3 pins GPIO
	PORTB_Base_PTR->PCR[RED_LED_POS] &= ~PORT_PCR_MUX_MASK;          
	PORTB->PCR[RED_LED_POS] |= PORT_PCR_MUX(1);          
	PORTB->PCR[GREEN_LED_POS] &= ~PORT_PCR_MUX_MASK;          
	PORTB->PCR[GREEN_LED_POS] |= PORT_PCR_MUX(1);          
	PORTD->PCR[BLUE_LED_POS] &= ~PORT_PCR_MUX_MASK;          
	PORTD->PCR[BLUE_LED_POS] |= PORT_PCR_MUX(1);          
	
	// Set ports to outputs
	PTB->PDDR |= MASK(RED_LED_POS) | MASK(GREEN_LED_POS);
	PTD->PDDR |= MASK(BLUE_LED_POS);
}

void Control_RGB_LEDs(unsigned int red_on, unsigned int green_on, unsigned int blue_on) {
	if (red_on) {
			PTB-> PCOR = MASK(RED_LED_POS);  			 //use PCOR to turn on the red LED
	} else {
			PTB->	PSOR = MASK(RED_LED_POS);				 //use PSOR to turn off the red LED
	}
	if (green_on) {
			PTB->	PCOR = MASK(GREEN_LED_POS);			 //use PCOR to turn on the green LED
	}	else {
			PTB-> PSOR = MASK(GREEN_LED_POS);			 //use PSOR to turn off the green LED
	} 
	if (blue_on) {
			PTD -> PCOR = MASK(BLUE_LED_POS); 		 //use PCOR to turn on the blue LED
	}	else {
			PTD -> PSOR = MASK(BLUE_LED_POS);  		 //use PSOR to turn off the blue LED
	}
}	
// ********************************************************************   

// PCOR turn on
