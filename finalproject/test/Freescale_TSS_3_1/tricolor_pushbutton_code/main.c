/*CEE-345 Microprocessor System Design
This code is to demonstrate the use of General Purpose Input and Output
ports on the Kinetic ARM processor. Three Tactile switches are used to control
a three color (RGB) LED on the Freedom board. By pressing each tact switch, users
will see RED, GREEN, and BLUE light on the LED.
 *----------------------------------------------------------------------------*/
#include <MKL25Z4.H>
#include "gpio_defs.h"
#include "LEDs.h"
#include "switches.h"

void Test_Switches(void) {
	// switch_code;              declare a parameter/variable: unsigned type 
	unsigned switch_code;
	while (1) {
		switch_code = ~READ_SWITCHES;        	   //read values from READ_SWITCHES variable
								   //RGB LEDs are turned on based on which of the tactile swicth
								   //is pressed; use MASK function to capture the pins needed
								   // to turn on the LED
		Control_RGB_LEDs( (switch_code & MASK(SW_UP_POS)),    
				(switch_code & MASK(SW_RT_POS)),		
				(switch_code & MASK(SW_LT_POS)) );
	}
}

/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {
	
	 //add code here to initialize the tactile swicthes
	 //add code here to initialize the LEDs
	 //Read the switch value from the Test_Switches function
		Init_5way_Switch();
	  Init_RGB_LEDs();
		Test_Switches();
}
 
