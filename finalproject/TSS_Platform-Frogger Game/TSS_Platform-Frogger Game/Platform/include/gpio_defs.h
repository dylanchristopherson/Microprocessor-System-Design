#ifndef GPIO_DEFS_H
#define GPIO_DEFS_H

//Define a macro for left shifting operation; 
// UL: Unsigned Long (data type);

#define MASK(x) (1UL << (x))  

// Freedom KL25Z LEDs
#define RED_LED_POS (18)		// on port B
#define GREEN_LED_POS (19)	// on port B
#define BLUE_LED_POS (1)		// on port D

#define R1 (1) // Red 1
#define R2 (2) // Red 2
#define G1 (3) // Green 1
#define G2 (4) // Green 2
#define B1 (5) // Blue 1
#define B2 (6) // Blue 2

#define clk (0) // Clock
#define lat (13) // ?

#define a (10) // Row Pins
#define b (11) // Row Pins
#define c (12) // Row Pins

#define OE (16) // ?

#define AllPins (MASK(R1) | MASK(R2) | MASK(G1) | MASK(G2) | MASK(B1) | MASK(B2) | MASK(clk) | MASK(lat) | MASK(a) | MASK(b) | MASK(c) | MASK(OE))
#define AllColorPins (MASK(R1) | MASK(R2) | MASK(G1) | MASK(G2) | MASK(B1) | MASK(B2))
#define AllColPins (MASK(a) | MASK(b) | MASK(c))

#endif
