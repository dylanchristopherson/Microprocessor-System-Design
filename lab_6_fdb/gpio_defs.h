#ifndef GPIO_DEFS_H
#define GPIO_DEFS_H

// define a macro for left shifting operation;
// UL: unsigned long (data type);

#define MASK(x) (1UL << (x))

// Freedom KL25Z LEDS
#define RED_LED_POS (18)		// on port B
#define GREEN_LED_POS (19)	// on port B
#define BLUE_LED_POS (1)		// on port D

#endif