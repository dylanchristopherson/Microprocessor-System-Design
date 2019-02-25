/*
 * C_lab_2.c
 *
 * Created: 2/6/2019 3:10:17 PM
 * Author : Student
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

//////////Knight Rider//////////

/*
int main(void)
{
	//Initializes registers
	DDRB = 0xff;
	PORTB = 0xff;
	
	// Initialize pattern to an 8 bit unsigned register
	uint8_t pattern = 1;
	int flag = 1;
	
	while(1)
	{
		//Send the negation of pattern to PORTB
		PORTB = ~pattern;
		
		// If flag is 1, shift pattern left one bit
		if (flag ==	1)
		{
			//Shifts bit left once
			pattern <<= 1;
		}
		
		// Else, shift pattern right one bit
		else 
		{
			//Shifts bit right once
			pattern >>= 1;
		}
		
		// If patter equals 128, set flag to 0 to reverse the direction of the bitwise shift
		if (pattern == 128)
			flag = 0;
		
		// If patter equals 1, set flag to 1 to reverse the direction of the bitwise shift	
		if (pattern == 1)
			flag = 1;
			
		// Delay the program 100 ms	
		_delay_ms(100);
		
	}
}
*/


//////////Traffic Light Controller//////////

//This program was built on the schematic in Lab #2

//*
const uint8_t tab[] PROGMEM = {0x0c,0x14,0x04,0x21,0x22,0x20};

int main(void)
{
	
	//Creates an unsigned 8 bit register
	uint8_t led = 0;	
	
	// Initializes registers
	DDRB = ~led;
	PORTB = ~led;
	
	// do loop continually runs traffic light system
	do 
	{
		// for loop to select the available options for the traffic lights
		for (int i = 0; i < 5; i++)
		{
			
			// index 1 contains a yellow light
			if (i == 1) {
				//This for loop blinks the yellow light
				for (int j = 0; j < 5; j++) {
					led = pgm_read_byte (&tab[1]);
					PORTB = ~led;
					_delay_ms(200);			
					
					led = pgm_read_byte (&tab[2]);
					PORTB = ~led;
					_delay_ms(200);
				}
			}
			
			// index 4 contains a yellow light
			else if (i == 4) {
				
				//This for loop blinks the yellow light
				for (int j = 0; j < 5 ; j++) {
					led = pgm_read_byte (&tab[4]);
					PORTB = ~led;
					_delay_ms(200);
					
					led = pgm_read_byte (&tab[5]);
					PORTB = ~led;
					_delay_ms(200);		
				}
			}
			
			// if there are no yellow lights, flash lights normally
			else {
				// grabs information about which lights are on
				led = pgm_read_byte (&tab[i]);
				// sends information to the output port
				PORTB = ~led;
				// delays 2 seconds
				_delay_ms(2000);	
			}
		}
		
	} while (1);	
}
