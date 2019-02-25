/*
 * C_lab_3_keypad.c
 *
 * Created: 2/11/2019 3:01:22 PM
 * Author : Student
 */ 

#include <avr/io.h>
#define F_CPU 4000000UL
#include <util/delay.h>
#include <stdlib.h>


char key_code[]={0xe7,0xee,0xde,0xbe,0xed,0xdd,0xbd,0xeb,0xdb,0xbb,0x7e,0x7d,0x7b,0x77,0xb7,0xd7};

void delay(long value)
{
	while(--value);
}

char scan()
{
	char temp, key, i;
	PORTD=0x0f;
	
	do {
		temp=PIND;			// Reads value from PIN D		
	} while (temp != 0x0f);
	
	// Checks which key is pressed on the rows
	while (1) {
		for(i = 0; i <=3; i++) {
			temp = ~(0x80 >>i);     //~(0b1000 0000)
			PORTD=temp;
			key=PIND;
			if(key!=temp)
			{
				delay(20);
				key=PIND;
				if(key!=temp) return (key);
			}
		}
	}	
}

// test if a key was pressed
char gotkey()
{
	char temp;
	int i;
	temp=scan();
	for (i=0; i<=15; i++) 
		if (temp == key_code[i]) return(i);
	return(16);
}

int main(void)
{
    char keys;
	// Port B initialization
	
	PORTB=0x00;
	DDRB=0xff;	// outputs connected to leds
	
	// Port D initialization
	PORTD=0x00;
	DDRD=0xF0;		// portd (7-4) keypad outputs
					// portd (3-0) keypad outputs
	while (1)
	{
		// Place your code here
		keys = gotkey();
		
		while (keys > 0) {
			PORTB=0xFF;
			_delay_ms(200);
			PORTB=0x00;
			_delay_ms(200);
			keys=keys-1;	
		}	
	}
}

