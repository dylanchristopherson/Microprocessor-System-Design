/*
* Chip type            : ARM Cortex-M0+
* Program type         : Firmware
* Core Clock Frequency : 48.00000 MHz
*/

#include <MKL25Z4.H>
#include "gpio_defs.h"

void Delay(unsigned int time_del) {
	// delay is about 1 millisecond * time_del
	volatile int t;
	
	while (time_del--) {
		for (t=4800; t>0; t--)
		;
	}
}

/*	Each LED corresponds to a bit on a port
		RED LED connected to PORT B (PTB), bit 18 (RED_LED_POS)
		Green LED connected to Port B (PTB), bit 19 (GREEN_LED_POS)
		Blue LED connected to Port D (PTD), bit 1 (BLUE_LED_POS)
		Active-Low outputs: Write a 0 to turn on an LED

		Turning LEDs on and off
		Turn on one LED: PTx->PDOR = ~ MASK(yyy_LED_POS);
		Turn on two LEDS: PTx ->PDOR = ~ ( MASK(yyy_LED_POS) | MASK(zzz_LED_POS)); 
*/

void red(void)
{
	// turn on red led
	PTB->PDOR = ~ MASK(RED_LED_POS);
	// turn off blue LED
	PTD->PDOR = 0xFFFFFFFF;					// This is a 32 bit register...
	
	// wait for 100ms
	Delay(100);
	
	// turn off red/green/blue LEDS
	PTB->PDOR = 0xFFFFFFFF;
	// turn off blue LED
	PTD->PDOR = 0xFFFFFFFF;
	
	// wait for 100ms
	Delay(100);
	
	// turn on red led
	PTB->PDOR = ~ MASK(RED_LED_POS);
	// turn off blue LED
	PTD->PDOR = 0xFFFFFFFF;
	
	// wait for 100ms
	Delay(100);
}

int main (void) {
	
	
	unsigned int counter = 0; // global variable to be used in debug mode
	
	/* Config steps
			1) Enable clock to GPIO ports
			2) Enable GPIO ports (2 steps process)
			3) Set GPIO direction to output
	    4) Ensure LEDS are off 
	*/
	
	// Enable clock to ports B and D
	// The clocks for PORT B and D are switched on
	
	/*
		The symbol SIM_SCGC5 defines the address of teh fifth
		system clock rating register to be at 0x40048038. This and 
		other symbols that represent register addresses (or masks that
		set or clear specific bits) are defined in the MCU-specific header
		file, MKL25Z4.h. This header file is provided with the 
		development tools. 
	*/
	
	
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;
	
	PORTB->PCR[RED_LED_POS] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RED_LED_POS] |= PORT_PCR_MUX(1);
	
	// Set ports to outputs: Port Data Direction Register (PDDR)
	
	PTB->PDDR |= MASK(RED_LED_POS);
	
	// Turn off LEDs: Port Set Output Register (PSOR)
	
	PTB->PSOR = MASK(RED_LED_POS);
	
	// end of configuration code
	
	// Code for flashing the LEDs
	
	while (1) {
		red();
	}
}
	
	