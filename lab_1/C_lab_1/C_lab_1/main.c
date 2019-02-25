/*
 * lab_1_c_code.c
 *
 * Created: 2/4/2019 3:12:52 PM
 * Author : Student
 */ 

//#include <avr/io.h>
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	
	DDRB = 0xff;
	PORTB = 0xff;
	//int pattern = 1;
	uint8_t pattern = 1;

	while(1)
	{
		PORTB = ~pattern;
		pattern <<=1;

		if (pattern == 0)
			pattern = 1;
		_delay_ms(100);	
		
		
	}	
	return 0;
}

