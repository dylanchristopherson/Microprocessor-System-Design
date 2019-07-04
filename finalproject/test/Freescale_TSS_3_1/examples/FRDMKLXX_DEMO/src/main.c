/*HEADER********************************************************************
*
* Copyright (c) 2013 Freescale Semiconductor;
* All Rights Reserved
*
***************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
**************************************************************************
*
* Comments:
*
*   This file contains the source for the TSS example program.
*
*END************************************************************************/

#include <MKL25Z4.H>
#include "TSS_API.h"
#include "freemaster.h"
#include "app_init.h"
#include "events.h"
#include "main.h"
#include "board.h"

uint16_t u16LPcounter = 0u;

// LEDs.h
#define RED_LED_POS (18)		// on port B
#define GREEN_LED_POS (19)	// on port B
#define BLUE_LED_POS (1)		// on port D


//switches.h
// All switches are on port E
#define SW_UP_POS (21)
#define SW_DN_POS (29)
#define SW_LT_POS (30)
#define SW_RT_POS (23)
#define SW_CR_POS (22)

// Macro to read switches returns state switches, active low
#define READ_SWITCHES  (PTE_BASE_PTR->PDIR)  


//gpio_defs.h
// basic light switch 
#define LED1_POS (1)	// on port A
#define LED2_POS (2)	// on port A
#define SW1_POS (5)		// on port A

// Define a Macro to do left shifting where UL is unsigned long - 32 bits
#define MASK(x) (1UL << (x))



void Init_5way_Switch(void);
void Init_RGB_LEDs(void);
void Test_Switches(void);


/*********************** GUI - FreeMASTER TSA table ************************/

#if FMSTR_USE_TSA
  /* Example of FreeMASTER User Application Table */
  FMSTR_TSA_TABLE_BEGIN(UserAppTable)
    FMSTR_TSA_RW_VAR(u16LPcounter, FMSTR_TSA_UINT8)
  FMSTR_TSA_TABLE_END()

  /* FreeMASTER Global Table List definition */
  FMSTR_TSA_TABLE_LIST_BEGIN()
    #if TSS_USE_FREEMASTER_GUI
      FMSTR_TSA_TABLE(TSS_Table)
    #endif
    FMSTR_TSA_TABLE(UserAppTable)
  FMSTR_TSA_TABLE_LIST_END()
#endif

/**
 * \brief  Main function
 *
 * \param  void
 *
 * \return int
 */

int main (void)
{
	Init_5way_Switch();
	
  InitPorts(); /* Default TSS init */
  
  TSS_Init_ASlider(); /* Init PWM for LED control */
  
  PWM_init();											// I believe this may be causing issues
  #if TSS_USE_FREEMASTER_GUI			// This statement is decided at compile time
    FreeMASTER_Init();						// I believe this is causing issues
  #endif
  /* Enable Interrupts globally */
  EnableInterrupts();
  /* Reset Low Power Counter flag */
  u16LPcounter = 0u;

 for(;;)
  {
    #if TSS_USE_FREEMASTER_GUI
      FMSTR_Poll();
    #endif
  
    if (TSS_Task() == TSS_STATUS_OK)
    {
      #if (LOW_POWER_TSI)			/* If 1 */
        LowPowerControl();
      #endif
    }
		Test_Switches();
	}
}








// LEDs.c

void Init_RGB_LEDs(void) {
	// Enable clock to ports B and D
	  SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;;
	
	// Make 3 pins GPIO
	// Port dec must be "PORTB_BASE_PTR->PTR" to work with TSS
	PORTB_BASE_PTR->PCR[RED_LED_POS] &= ~PORT_PCR_MUX_MASK;          
	PORTB_BASE_PTR->PCR[RED_LED_POS] |= PORT_PCR_MUX(1);          
	PORTB_BASE_PTR->PCR[GREEN_LED_POS] &= ~PORT_PCR_MUX_MASK;          
	PORTB_BASE_PTR->PCR[GREEN_LED_POS] |= PORT_PCR_MUX(1);          
	PORTB_BASE_PTR->PCR[BLUE_LED_POS] &= ~PORT_PCR_MUX_MASK;          
	PORTB_BASE_PTR->PCR[BLUE_LED_POS] |= PORT_PCR_MUX(1);          
	
	// Set ports to outputs
	PTB_BASE_PTR->PDDR |= MASK(RED_LED_POS) | MASK(GREEN_LED_POS);
	PTD_BASE_PTR->PDDR |= MASK(BLUE_LED_POS);
}

void Control_RGB_LEDs(unsigned int red_on, unsigned int green_on, unsigned int blue_on) {
	// Red
	if (red_on) {
		SET_LED_RED(cASlider1.Position);
		SET_LED_BLUE(0);
		SET_LED_GREEN(0);
	} 
	else if (green_on) {
		SET_LED_RED(0);
		SET_LED_BLUE(0);
		SET_LED_GREEN(cASlider1.Position);
	}
	else if (blue_on) {
		SET_LED_RED(0);
		SET_LED_BLUE(cASlider1.Position);
		SET_LED_GREEN(0);
	}	
	
	else {
		  SET_LED_RED(cASlider1.Position);					
      SET_LED_GREEN(cASlider1.Position);
      SET_LED_BLUE(cASlider1.Position);
	}
}	

		//	PTB_BASE_PTR->	PSOR = MASK(GREEN_LED_POS);			 // use PSOR to turn off the green LED
		//  PTD_BASE_PTR -> PSOR = MASK(BLUE_LED_POS);			 // use PSOR to turn off the blue LED
		//  PTB_BASE_PTR-> PCOR = MASK(RED_LED_POS);  			 //use PCOR to turn on the red LED

//else {
			// PTB_BASE_PTR->	PSOR = MASK(RED_LED_POS);				 //use PSOR to turn off the red LED
	//}
	// Green

		 // PTB_BASE_PTR->	PSOR = MASK(RED_LED_POS);	       // use PSOR to turn off the red LED
		 // PTD_BASE_PTR -> PSOR = MASK(BLUE_LED_POS);       // use PSOR to turn off the blue LED
		//	PTB_BASE_PTR->	PCOR = MASK(GREEN_LED_POS);			 //use PCOR to turn on the green LED

//	else {
			// PTB_BASE_PTR-> PSOR = MASK(GREEN_LED_POS);			 //use PSOR to turn off the green LED
	//} 
	// Blue

		//  PTB_BASE_PTR->	PSOR = MASK(RED_LED_POS);      // use PSOR to turn off the red LED
		//  PTB_BASE_PTR->	PSOR = MASK(GREEN_LED_POS);		 // use PSOR to turn off the green LED
		//  PTD_BASE_PTR -> PCOR = MASK(BLUE_LED_POS); 		 //use PCOR to turn on the blue LED

//else {
			// PTD_BASE_PTR -> PSOR = MASK(BLUE_LED_POS);  		 //use PSOR to turn off the blue LED
	//}

// ********************************************************************   


// Switches.c

void Init_5way_Switch(void) {
  SIM_SCGC5 |=  SIM_SCGC5_PORTE_MASK; /* enable clock for port E */
	
  /* Select GPIO and enable pull-up resistors for pins connected to switches */
  PORTE_BASE_PTR->PCR[SW_UP_POS] |= PORT_PCR_MUX(1) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK;	
  PORTE_BASE_PTR->PCR[SW_DN_POS] |= PORT_PCR_MUX(1) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK;	
  PORTE_BASE_PTR->PCR[SW_LT_POS] |= PORT_PCR_MUX(1) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK;	
  PORTE_BASE_PTR->PCR[SW_RT_POS] |= PORT_PCR_MUX(1) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK;	
  PORTE_BASE_PTR->PCR[SW_CR_POS] |= PORT_PCR_MUX(1) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK;	
	
  /* Set port E bits 0-3, 7 to inputs */
  PTE_BASE_PTR->PDDR &= ~( MASK(SW_UP_POS) | 
				  MASK(SW_DN_POS) |
				  MASK(SW_LT_POS) | 
				  MASK(SW_RT_POS) |
				  MASK(SW_CR_POS) );
}

void Test_Switches(void) {
	// switch_code;              declare a parameter/variable: unsigned type 
	unsigned switch_code;
	
		switch_code = ~READ_SWITCHES;        	   //read values from READ_SWITCHES variable
								   //RGB LEDs are turned on based on which of the tactile swicth
								   //is pressed; use MASK function to capture the pins needed
								   // to turn on the LED
//					Control_RGB_LEDs(0,0,1);
				Control_RGB_LEDs( (switch_code & MASK(SW_UP_POS)),    
				(switch_code & MASK(SW_RT_POS)),		
				(switch_code & MASK(SW_LT_POS)) );

}

//int main (void) {
	
	
 // InitPorts(); /* Default TSS init */
 
//  TSS_Init_ASlider(); /* Init PWM for LED control */
  
//  PWM_init();	
//	Init_5way_Switch();
//	while(1) {
		//SET_LED_RED(1);
	 //add code here to initialize the tactile swicthes
	 //add code here to initialize the LEDs
	 //Read the switch value from the Test_Switches function

//	  Init_RGB_LEDs();
//	Test_Switches();
//	}
//}

