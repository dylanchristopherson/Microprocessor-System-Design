// Part 1 Info

/* Matrix keypad detect

* This program checks a 4x4 matrix keypad to see whether
 * a key is pressed or not.  When a key is pressed, it turns
 * on the blue LED.
 *
 * PortC 7-4 are connected to the columns and PortC 3-0 are connected
 * to the rows.
 */

// Part 2 Info

/*
	Matrix keypad scanning
	
	This program scans a 4x4 matrix keypad and returns a unique number
	for each key pressed. The number will turn on/off the red and green 
	LEDs.
	
							FRDM Pin
	row 0 PTC10 J1-13
	row 1 PTC11 J1-15
	row 2 PTC12 J1-1
	row 3 PTC13 J1-3
	row 0 PTE20 J1-1
	row 1 PTE21 J1-3
	row 2 PTE22 J1-5
	row 3 PTE23 J1-7
*/	


#include <MKL25Z4.H>
#include "gpio_defs.h"

// Part 1 Functions

void delayUs(int n);
void keypad_init(void);
char keypad_kbhit(void);

// Part 2 Functions

void delayMs(int n);
char keypad_getkey(void);
void LED_init(void);
void LED_set(char value);

int main(void)
{
	unsigned char key;
	
	keypad_init();
	LED_init();
	
	while(1) {
		key = keypad_getkey();
		
		if (key == '1')
			LED_set('1');
		if (key == '2')
			LED_set('2');
		if (key == 'E')
			LED_set('E');
		if (key == 'D')
			LED_set('D');
		
		
	}
		
/* Blue LED with keypad */	
//    keypad_init();
//    SIM->SCGC5 |= 0x1000;       /* enable clock to Port D */
//    PORTD->PCR[1] = 0x100;      /* make PTD1 pin as GPIO */
//    PTD->PDDR |= 0x02;          /* make PTD1 as output pin */
   
//    while(1)
//    {
//        if (keypad_kbhit() != 0)    /* if a key is pressed? */
          //  PTD->PCOR |= 0x02;          /* turn on blue LED */
//				    PTD->PDOR =  ~MASK(BLUE_LED_POS); /* turn on blue LED */
//        else
           // PTD->PSOR |= 0x02;          /* turn off blue LED */
//				   PTD->PDOR =  MASK(BLUE_LED_POS); /* turn off blue LED */

}





/* this function initializes PortC that is connected to the keypad.
 * All pins are configured as GPIO input pin with pullup enabled.
 */


// Part 1 Functions

void keypad_init(void)
{
    SIM->SCGC5 |= 0x0800;       /* enable clock to Port C */
    SIM->SCGC5 |= 0x2000;       /* enable clock to Port E */
    
	
		// Port control register #
	
    PORTC->PCR[10] = 0x100;     /* make PTC10 pin as GPIO */
    PORTC->PCR[11] = 0x100;     /* make PTC11 pin as GPIO */
    PORTC->PCR[12] = 0x100;     /* make PTC12 pin as GPIO */
    PORTC->PCR[13] = 0x100;     /* make PTC13 pin as GPIO */
    
    PORTE->PCR[20] = 0x100;     /* make PTE20 pin as GPIO */
    PORTE->PCR[21] = 0x100;     /* make PTE21 pin as GPIO */
    PORTE->PCR[22] = 0x100;     /* make PTE22 pin as GPIO */
    PORTE->PCR[23] = 0x100;     /* make PTE23 pin as GPIO */
  
	/* for blue blinking LED */
//		SIM->SCGC5 |= 0x0800;       /* enable clock to Port C */ 
//    PORTC->PCR[0] = 0x103;      /* make PTD0 pin as GPIO and enable pullup*/
//    PORTC->PCR[1] = 0x103;      /* make PTD1 pin as GPIO and enable pullup*/
//    PORTC->PCR[2] = 0x103;      /* make PTD2 pin as GPIO and enable pullup*/
//    PORTC->PCR[3] = 0x103;      /* make PTD3 pin as GPIO and enable pullup*/
//    PORTC->PCR[4] = 0x103;      /* make PTD4 pin as GPIO and enable pullup*/
//    PORTC->PCR[5] = 0x103;      /* make PTD5 pin as GPIO and enable pullup*/
//    PORTC->PCR[6] = 0x103;      /* make PTD6 pin as GPIO and enable pullup*/
//    PORTC->PCR[7] = 0x103;      /* make PTD7 pin as GPIO and enable pullup*/
//    PTD->PDDR = 0x0F;         /* make PTD7-0 as input pins */

}

/* This is a non-blocking function.
 * If a key is pressed, it returns 1.
 * Otherwise, it returns a 0 (not ASCII '0'). */
char keypad_kbhit(void)
{
    int col;

    PTC->PDDR |= 0x0F;          /* enable column (PTC4-PTC7) as input */
																/* enable row (PTC0-PTC3) as output */
    PTC->PCOR = 0x0F;           /* enable all rows driven low */
    delayUs(2);                 /* wait for signal return */
    col = PTC->PDIR & 0xF0;     /* read all columns */
																/* PDOR has a vaue of 0xF0 when no key pressed */
																
    PTC->PDDR = 0;              /* disable all rows */
    if (col == 0xF0)
        return 0;               /* no key pressed */
    else
        return 1;               /* a key is pressed */
}

void delayUs(int n)
{
    int i; int j;
    for(i = 0 ; i < n; i++) {
        for(j = 0; j < 5; j++) ;
    }
}

// Part 2 Functions

// void delayMs(int n);

char keypad_getkey(void) {
	int row, col;

	const char col_select[] = {0x01, 0x02, 0x04, 0x08};
	const char keymap[] = " 123A456B789C0FED";
	
	/* check to see if any key pressed first */
	PTE->PDDR |= 0xF << 20;	        /* enable columns as outputs (PTE20-23)*/
	PTE->PSOR = 0xF << 20;					/* set column data pins all high in PDOR for PTE */
	delayUs(2);											/* wait for signal to settle */
	row = (PTC->PDIR >> 10) & 0xF;	/* read all rows at PORT C (PTC10-13) */
	PTE->PCOR = 0xF << 20;					/* set column data pins all low */
	PTE->PDDR &= ~ (0xF << 20); 		/* disable all columns */
	
	if (row == 0) return 0; /* no key pressed, return a zero */

	for (col = 0; col <= 4; col++) {
		PTE->PDDR &= ~(0xF << 20);
		PTE->PDDR |= col_select[col] << 20;
		PTE->PSOR = 0x0F << 20;
		delayUs(2);
		row = (PTC-> PDIR >> 10) & 0xF;
		PTE->PCOR = 0xF << 20;
		if (row != 0) break;
  }
	
	PTE->PDDR &= ~(0xF << 20); /* disable all cols */
	
	if(col == 4) return 0;		/* if we get here, no key is pressed */
	
	if (row == 0x01) return keymap[col + 0 * 4 + 1];	// key in column 0
	if (row == 0x02) return keymap[col + 1 * 4 + 1];	// key in column 1
	if (row == 0x04) return keymap[col + 2 * 4 + 1];	// key in column 2
	if (row == 0x08) return keymap[col + 3 * 4 + 1];	// key in column 3
	
	return 0; /* just to be safe */
}

/* initialize all three LEDs on the FRDM board */
void LED_init(void)
{
    SIM->SCGC5 |= 0x400;        /* enable clock to Port B */
    SIM->SCGC5 |= 0x1000;       /* enable clock to Port D */
    PORTB->PCR[18] = 0x100;     /* make PTB18 pin as GPIO */
    PTB->PDDR |= 0x40000;       /* make PTB18 as output pin */
    PTB->PSOR |= 0x40000;       /* turn off red LED */
    PORTB->PCR[19] = 0x100;     /* make PTB19 pin as GPIO */
    PTB->PDDR |= 0x80000;       /* make PTB19 as output pin */
    PTB->PSOR |= 0x80000;       /* turn off green LED */
    PORTD->PCR[1] = 0x100;      /* make PTD1 pin as GPIO */
    PTD->PDDR |= 0x02;          /* make PTD1 as output pin */
    PTD->PSOR |= 0x02;          /* turn off blue LED */
}


void LED_set(char value) {
    
    if (value == '1')               /* use bit 0 of value to control red LED */
        PTB->PCOR = 0x40000;    /* turn on red LED */
    else if (value == '2')
        PTB->PSOR=0x40000;      /* turn off red LED */
    else if (value == 'E')
        PTB->PCOR = 0x80000;    /* turn on green LED */
    else if (value == 'D')
        PTB->PSOR = 0x80000;    /* turn off green LED */
}


