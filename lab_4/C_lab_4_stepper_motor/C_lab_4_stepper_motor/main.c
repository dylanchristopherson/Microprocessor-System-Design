/*
 * C_lab_4_stepper_motor.c
 *
 * Created: 2/20/2019 11:06:10 AM
 * Author : Student
 */ 

#include <avr/io.h>
#define F_CPU 4000000UL
#include <util/delay.h>
#include <stdlib.h>

// a sequence digital pulses is written into an array for stepper to run forward
// reversed the sequence of digital pulses for the opposite direction
// a method for scanning the keypad and returning the key is needed

char key_code[] = {0xE7, 0xEE, 0xDE, 0xBE, 0xED, 0xDD, 0xBD, 0xEB,
				   0xDB, 0xBB, 0x7E, 0x7D, 0x7B, 0x77, 0xB7, 0xD7};

// sequence for motor running forward
char forward[] = {0x08, 0x0C, 0x04, 0x06, 0x02, 0x03, 0x01, 0x09};

// sequence for motor running backwards
char backward[] = {0x09, 0x01, 0x03, 0x02, 0x06, 0x04, 0x0C, 0x08};

void delay(long value)	// delay used to control the speed of the motor
{
	while(--value);
}

// scan for pressed key
char scan()
{
	char temp, key, i;	
	PORTD = 0x0f;
	do {
		temp=PIND;
	} while (temp!=0x0f);
	
	while(1)
	{
		for (i=0; i<= 3;i++) {
			temp = ~(0x80 >>i);
			PORTD=temp;
			key=PIND;
			if(key!=temp)
			{		
				delay(20);
				key=PIND;
				if(key!=temp) return(key);
			}	
		}	
	}
}

char gotKey()
{
	char temp;
	int i;
	temp=scan();
	for (i=0; i<=15; i++)
		if (temp==key_code[i]) return(i);
	return(16);
}

// run motor forward
void runForward()
{
	int i = 0;
	int j = 0;
	
	while(j<300)				// motor runs for 200 iterations of the hex sequence
	{
		_delay_ms(10);			// delay adjusts the speed of the motor, picked by user
		PORTC = forward[i];		// send motor the next hex value from the sequence
		
		i=i+1;				// inc i
		j=j+1;				// inc j
		if(i == 8) i=0;   	// reset i back to 0 when it reaches the end of the sequence
	}
}

// run motor forward
void runBackward()
{
	int i = 7;
	int j = 0;
	
	while(j<300)				// motor runs for 200 iterations of the hex sequence
	{
		_delay_ms(4);			// delay adjusts the speed of the motor, picked by user
		PORTC = forward[i];		// send motor the next hex value from the sequence
		
		i--;				// dec i
		j++;				// inc j
		
		if(i == 0) i=7;		// reset i back to 0 when it reaches the end of the sequence
	}
}

int main(void)
{
	char keys;
	
	PORTC=0x00;   // Port C initialization
	DDRC=0xFF;    // Port C is the motor input
	
	PORTD=0x00;   // Port D initialization
	DDRD=0xF0;	  // Port D is the keypad

	while(1)
	{
		keys=gotKey();
		
		if (keys == 0x0B) runBackward();   // if keys equals B
			
		if (keys == 0x0F) runForward();	   // if keys equals F		
	}
}

 