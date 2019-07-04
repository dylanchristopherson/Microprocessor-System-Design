#ifndef GPIO_DEFS_H
#define GPIO_DEFS_H

// basic light switch 
#define LED1_POS (1)	// on port A
#define LED2_POS (2)	// on port A
#define SW1_POS (5)		// on port A

// Define a Macro to do left shifting where UL is unsigned long - 32 bits
#define MASK(x) (1UL << (x))

// Speaker output   //speaker is not used in this lab
#define SPKR_POS (0) 	// on port C

#endif
// ********************************************************************   
