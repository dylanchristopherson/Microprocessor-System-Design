/*----------------------------------------------------------------------------
This project code demonstrates how to interface a ARM Cortex M0+ processor with
a LCD driver IC controller
*----------------------------------------------------------------------------*/
#include <MKL25Z4.H>
#include "gpio_defs.h"
#include "LCD_4bit.h"

/* create a delay function */
void Delay(volatile unsigned int time_del) {
	while (time_del--) {
		;
	}
}

/*------------------------------------------------------------------------------
	Example for LCD interface
	*------------------------------------------------------------------------------*/
void LCD_Example(void) {
	//use a function call to initialize the LCD 
	Init_LCD();
	// use a function call to Clear_LCD();
	Clear_LCD();
	Set_Cursor(0,0);
	Print_LCD("Hello CEE-345"); //Must have 16 letter space
	//Print_LCD(" Hello, World!:)"); //Must have 16 letter space
	Set_Cursor(0,1);
	Print_LCD("First LCD Test"); //Must have 16 letter space
}

/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {
	LCD_Example();
	
}
