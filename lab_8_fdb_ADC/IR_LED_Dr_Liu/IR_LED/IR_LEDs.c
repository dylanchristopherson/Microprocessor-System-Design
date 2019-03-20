#include <MKL25Z4.H>
#include "user_defs.h"
#include "LEDs.h"

//This program is to light RGB LED according to range of the IR LED.

//the Threshold parameters are used to sweep through valid IR Levels of the IR_LED
//IR_LED
int Threshold[NUM_RANGE_STEPS] = {1100, 650, 400, 270, 200, 0};

const int Colors[NUM_RANGE_STEPS][3] = {{ 1, 1, 1}, // white
																	{ 1, 0, 1}, // magenta
																	{ 1, 0, 0}, // red
																	{ 1, 1, 0}, // yellow
																	{ 0, 0, 1}, // blue
																	{ 0, 1, 0}};// green
																	 

void Init_ADC(void) {
	
	SIM->SCGC6 |= (1UL << SIM_SCGC6_ADC0_SHIFT); 
	ADC0->CFG1 = ADC_CFG1_ADLPC_MASK | ADC_CFG1_ADIV(0) | ADC_CFG1_ADLSMP_MASK; // 16 bit precision				// ADC0->CFG1 = ;
	ADC0->SC2 = 0;
}

//switch on and off IR_LED
void Control_IR_LED(unsigned int led_on) {
	if (led_on) {
			PTB->PCOR = MASK(IR_LED_POS);	// PCOR turns it off. Clears`
	} else {
			PTB->PSOR = MASK(IR_LED_POS); // PTB-> = MASK(IR_LED_POS);
	}
}	

//initilize IR_LED by using the PCR register; identify the pin number
//on the PORTB to connect it to PORTB 
void Init_IR_LED(void) {
	PORTB->PCR[IR_LED_POS] &= ~PORT_PCR_MUX_MASK;   // PORTB->[IR_LED_POS] &= ~PORT_PCR_MUX_MASK;            
	PORTB->PCR[IR_LED_POS] |= PORT_PCR_MUX(1);      //    PORTB->[IR_LED_POS] |= PORT_PCR_MUX(1);          
	PTB->PDDR |= MASK(IR_LED_POS);
	
	// start off with IR LED turned off
	Control_IR_LED(0);
}

unsigned Measure_IR(void) {
	volatile unsigned res=0;
	
	ADC0->SC1[0] = IR_PHOTOTRANSISTOR_CHANNEL; // start conversion on channel 0
	
	while (!(ADC0->SC1[0] & ADC_SC1_COCO_MASK))
		;
	res = ADC0->R[0];
	// complement result since voltage falls with increasing IR level
	
	// but we want result to rise with increasing IR level
	return 0xffff-res;
}

void Display_Range(int b) {
	unsigned i;
	
	for (i=0; i<NUM_RANGE_STEPS-1; i++) {
		if (b > Threshold[i])
			break;
	}
	Control_RGB_LEDs(Colors[i][RED], Colors[i][GREEN], Colors[i][BLUE]);
}

void Delay_us(volatile unsigned int time_del) {
	// This is a very imprecise and fragile implementation!
	time_del = 9*time_del + time_del/2; 
	while (time_del--) {
		;
	}
}
/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {
	unsigned on_brightness=0, off_brightness=0;
	static int avg_diff;
	int diff;
	unsigned n;
	
	Init_ADC();
	Init_RGB_LEDs();
	Init_IR_LED();
	Control_RGB_LEDs(0, 0, 0);
	
	while (1) {
		diff = 0;
		for (n=0; n<NUM_SAMPLES_TO_AVG; n++) {
			// measure IR level with IRLED off
			Control_IR_LED(0);
			Delay_us(T_DELAY);
			off_brightness = Measure_IR();
		
			// measure IR level with IRLED on
			Control_IR_LED(1);
			Delay_us(T_DELAY);
			on_brightness = Measure_IR();

			// calculate difference
			diff += on_brightness - off_brightness;
		}
		avg_diff = diff/NUM_SAMPLES_TO_AVG;
		
		// light RGB LED according to range
		Display_Range(avg_diff);
		
	}
}

// *******************************ARM University Program Copyright © ARM Ltd 2013*************************************   
