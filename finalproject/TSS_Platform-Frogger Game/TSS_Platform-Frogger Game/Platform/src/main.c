#include "MKL25Z4.h"
#include "TSS_API.h"
#include "freemaster.h"
#include "app_init.h"
#include "events.h"
#include "main.h"
#include "board.h"
#include "gpio_defs.h"
#define COLS 16
#define ROWS 32

// Port declarations must be changed in all locations to work with the TSS Library
// Normally, they are "PORTC->PCR", but need to be "PORTC_BASE_PTR->PCR" to work

uint16_t u16LPcounter = 0u;
char screen[ROWS][COLS];

int game_speed = 8; // 1 (slow) - 10 (fast) // too fast may not work (above 8)

char startGameScreen[COLS][ROWS] = { // PLATFORM | BY: CORY
{0,0,0,0,0,0,7,0,0,0,0,0,0,0,7,7,7,7,0,7,0,0,0,0,0,0,0,7,7,7,7,7},
{0,0,0,0,0,0,7,7,7,7,7,0,0,7,0,0,0,0,0,7,0,0,0,0,0,0,0,0,0,7,0,7},
{0,0,0,0,0,0,7,0,0,0,0,0,0,7,7,7,7,7,0,7,0,0,0,0,0,0,0,7,7,7,0,7},
{0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,7,7,7,7,0,0,0,0,7,7,7,7,7,0,7,0,0,0,0,0,0,0,7,7,0,7,7},
{0,0,0,0,7,0,7,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,0,7,0,7,0,0},
{0,0,0,0,0,7,7,7,7,0,0,7,7,0,7,7,0,0,0,7,0,0,7,0,7,0,0,7,7,7,7,7},
{0,0,0,0,0,0,0,0,0,0,0,7,0,7,0,0,0,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,7,0,0,0,0,7,7,7,7,7,0,0,0,7,0,7,7,7,7,7,0,7,7,7,7,7},
{0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,7,0,7,0,7,0,0,0,7},
{0,0,7,7,7,7,7,0,0,7,7,7,7,7,0,0,0,0,0,7,0,7,7,7,0,7,0,7,7,7,7,7},
{0,0,0,0,0,0,0,0,0,7,0,0,0,7,0,0,0,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0},
{7,7,7,0,0,0,0,0,0,7,7,7,7,7,0,0,0,0,0,7,0,0,7,0,7,0,0,7,0,0,0,7},
{7,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,7,0,7,0,7,0,7,0,0,0,7},
{7,0,7,0,0,0,0,7,0,7,0,0,0,0,0,0,0,0,0,7,0,7,7,7,7,7,0,7,7,7,7,7},
{7,7,7,7,7,0,0,7,7,7,7,7,0,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0}};
char youWinScreen[COLS][ROWS] = { // YOU WIN / RESET
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0},
{0,2,2,2,2,2,0,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2},
{0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0},
{0,0,0,0,0,2,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,2,2,2,2,2,0,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,2,0,2},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2},
{0,2,2,2,2,2,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,2,0,0,0,2,0,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,2,2,2},
{0,2,0,0,0,2,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,2},
{0,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,2,0,2},
{0,2,2,2,2,2,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2},
{0,0,0,2,0,2,0,0,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,2,0,2,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,2,2},
{0,2,2,2,0,2,0,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,2,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2}};
char gameOverScreen[COLS][ROWS] = { // GAME OVER / RESET
{1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
{1,1,1,1,1,0,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1},
{0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
{0,1,1,1,1,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1},
{1,1,1,1,1,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1},
{1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,1},
{0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1},
{0,1,1,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{1,0,1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1},
{0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1},
{0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{1,0,1,1,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1},
{1,0,0,0,1,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0},
{1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1}};
	
void InitLEDGameBoard(){ // Initialize clk/ports
	// Port declarations must be "PORTC_BASE_PTR->PCR" to work with TSS
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
	PORTC_BASE_PTR->PCR[R1] &= ~PORT_PCR_MUX_MASK;
	PORTC_BASE_PTR->PCR[R1] |= PORT_PCR_MUX(1);
	PORTC_BASE_PTR->PCR[R2] &= ~PORT_PCR_MUX_MASK;
	PORTC_BASE_PTR->PCR[R2] |= PORT_PCR_MUX(1);
	PORTC_BASE_PTR->PCR[G1] &= ~PORT_PCR_MUX_MASK;
	PORTC_BASE_PTR->PCR[G1] |= PORT_PCR_MUX(1);
	PORTC_BASE_PTR->PCR[G2] &= ~PORT_PCR_MUX_MASK;
	PORTC_BASE_PTR->PCR[G2] |= PORT_PCR_MUX(1);
	PORTC_BASE_PTR->PCR[B1] &= ~PORT_PCR_MUX_MASK;
	PORTC_BASE_PTR->PCR[B1] |= PORT_PCR_MUX(1);
	PORTC_BASE_PTR->PCR[B2] &= ~PORT_PCR_MUX_MASK;
	PORTC_BASE_PTR->PCR[B2] |= PORT_PCR_MUX(1);
	PORTC_BASE_PTR->PCR[a] &= ~PORT_PCR_MUX_MASK;
	PORTC_BASE_PTR->PCR[a] |= PORT_PCR_MUX(1);
	PORTC_BASE_PTR->PCR[b] &= ~PORT_PCR_MUX_MASK;
	PORTC_BASE_PTR->PCR[b] |= PORT_PCR_MUX(1);
	PORTC_BASE_PTR->PCR[c] &= ~PORT_PCR_MUX_MASK;
	PORTC_BASE_PTR->PCR[c] |= PORT_PCR_MUX(1);
	PORTC_BASE_PTR->PCR[clk] &= ~PORT_PCR_MUX_MASK;
	PORTC_BASE_PTR->PCR[clk] |= PORT_PCR_MUX(1);
	PORTC_BASE_PTR->PCR[lat] &= ~PORT_PCR_MUX_MASK;
	PORTC_BASE_PTR->PCR[lat] |= PORT_PCR_MUX(1);
	PORTC_BASE_PTR->PCR[OE] &= ~PORT_PCR_MUX_MASK;
	PORTC_BASE_PTR->PCR[OE] |= PORT_PCR_MUX(1);
	PTC_BASE_PTR->PDDR |= AllPins;
	PTC_BASE_PTR->PCOR = AllPins;
	PTC_BASE_PTR->PSOR = MASK(OE);
}

// Various functions used to control LED display pins
// Port declarations must be "PORTC_BASE_PTR->PCR" to work with TSS
void set(int pin)	{ PTC_BASE_PTR->PSOR = MASK(pin); }
void clear(int pin)	{ PTC_BASE_PTR->PCOR = MASK(pin); }
void pulse(int pin)	{ PTC_BASE_PTR->PTOR = MASK(pin);
					  PTC_BASE_PTR->PTOR = MASK(pin); }

// Delay function for the LED display
void delay(unsigned int time_del)
{
	volatile int t;
	while (time_del--) for (t=4; t>0; t--) ;
}

// setCols function to send data to the LED display
void setCols(int col)
{
	// Port declarations must be "PORTC_BASE_PTR->PCR" to work with TSS
	PTC_BASE_PTR->PCOR = AllColPins;
	switch(col)
	{
		case 0: case 8: //000
			PTC_BASE_PTR->PCOR = MASK(a) | MASK(b) | MASK(c); break;
		case 1: case 9: //001
			PTC_BASE_PTR->PCOR = MASK(b) | MASK(c); PTC_BASE_PTR->PSOR = MASK(a); break;
		case 2: case 10: //010
			PTC_BASE_PTR->PCOR = MASK(a) | MASK(c); PTC_BASE_PTR->PSOR = MASK(b); break;
		case 3: case 11: //011
			PTC_BASE_PTR->PCOR = MASK(c); PTC_BASE_PTR->PSOR = MASK(a) | MASK(b); break;
		case 4: case 12: //100
			PTC_BASE_PTR->PCOR = MASK(a) | MASK(b); PTC_BASE_PTR->PSOR = MASK(c); break;
		case 5: case 13: //101
			PTC_BASE_PTR->PCOR = MASK(b); PTC_BASE_PTR->PSOR = MASK(a) | MASK(c); break;
		case 6: case 14: //110
			PTC_BASE_PTR->PCOR = MASK(a); PTC_BASE_PTR->PSOR = MASK(b) | MASK(c); break;
		case 7: case 15: //111
			PTC_BASE_PTR->PSOR = MASK(a) | MASK(b) | MASK(c); break;
	}
}

// setColor function to change the color of the LED display
void setColor(int i, int j)
{
	if(screen[i][j] & 1)
		set(R1);	else	clear(R1);
	if(screen[i][j+8] & 1)
		set(R2);	else	clear(R2);
	if(screen[i][j] & 2)
		set(G1);	else	clear(G1);
	if(screen[i][j+8] & 2)
		set(G2);	else	clear(G2);
	if(screen[i][j] & 4)
		set(B1);	else	clear(B1);
	if(screen[i][j+8] & 4)
		set(B2);	else	clear(B2);
}

// drawScreen function to update the image on the LED display
void drawScreen()
{
	int i, j;
	for(j = 0; j < COLS/2; j++)
	{
		set(OE); setCols(j);
		for(i = 0; i < ROWS; i++)
		{ setColor(i, j); delay(2); pulse(clk); }
		pulse(lat); clear(OE); delay(200);
	}
	set(OE);
}

int main (void) {
	int i, j, k, delay, temp[16];			// Multi-purpose variables
	int curX, curY = 16, curPlat = 2;	// Cursor position variables
	int hasLost = 0, step = 0;				// Game state variables
	int platform[6][17] = {
		{ 7,7,0,0,7,7,7,7,7,7,7,7,7,7,7,7 },
		{ 7,7,7,7,7,7,7,7,0,0,7,7,7,7,7,7 },
		{ 7,7,7,7,7,7,7,7,7,7,7,7,0,0,7,7 },
		{ 7,7,7,7,0,0,7,7,7,7,7,7,7,7,7,7 },
		{ 7,7,7,7,7,7,7,7,7,7,7,0,0,7,7,7 },
		{ 7,7,7,7,7,0,0,7,7,7,7,7,7,7,7,7 }};
	
	InitLEDGameBoard();	// Initialize the LED game board ports
  TSS_Init_ASlider(); // Initialize "Touch Sensing Software"
	
	// Send the "Start Game Screen" image to LED display
	for(i = 0; i < ROWS; i++)
		for(j = 0; j < COLS; j++)
			screen[i][j] = startGameScreen[j][i];
		
	// Keep drawing the "Start Game Screen" until player touches slider
	curX = cASlider1.Position;
	while ( cASlider1.Position == curX )
		{ drawScreen(); TSS_Task(); }
	
	// Loop the game function until player wins or loses
	while(!hasLost)
	{
		// Loop speed depends on "game_speed" option defined above
		for(delay = 0; delay < (12-game_speed)*10; delay++)
		{
			// Clear screen to remove old artifacts
			for(i = 0; i < ROWS; i++)
				for(j = 0; j < COLS; j++)
					 screen[i][j] = 0;
			
			// Update touchpad information
			TSS_Task();
			
			// Draw the platforms on the screen (6 total)
			for(i = 0; i < 6; i++)
				for(j = i*5+2; j < i*5+4; j++)
						for (k = 0; k < COLS; k++)
							screen[j+5-step%5][k] = platform[i][k];
			
			// Draw the player's cursor on the screen (2x2)
			for(i = 0; i < COLS; i++)
			{
				if ( cASlider1.Position/14 > 13 ) curX = 0;
				else curX = 14 - cASlider1.Position/14;
				if ( i == curX || i == curX + 1 )
				{ screen[curY][i] = 4; screen[curY-1][i] = 4; }
			}
			
			// Drop the cursor to the platform below if it's in a gap
			if (platform[curPlat][curX] == 0 && platform[curPlat][curX+1] == 0)
			{ curY+=5; curPlat=(curPlat+1)%6; }
			
			// Add top (red) and bottom (green) bars
			for(j = 0; j < COLS; j++) 
			{	screen[0][j] = 1; screen[31][j] = 2; }
			
			// After creating all objects, draw the updated screen
			drawScreen();
		}
		
		// Increment the platform step (1-6) and raise the cursor with it
		step++; curY--;
		
		// If at top/bottom of screen, break loop
		if (curY==1) { hasLost = 1; break; }
		if (curY > 30) break;
			
		// Shift the platforms up to the level (and adjust cursor location)
		if (step % 5 == 0)
		{
			for (i = 0; i < COLS; i++)
				temp[i] = platform[0][i];
			for (j = 0; j < 5; j++)
				for (i = 0; i < COLS; i++)
					platform[j][i] = platform[j+1][i];
			for (i = 0; i < COLS; i++)
				platform[5][i] = temp[i];
			if (!curPlat) curPlat = 5; else curPlat--;
		}
	}
	
	// End-game state to repeat the "You Win" or "Game Over" screen forever
	while (1)
	{
		for(i = 0; i < ROWS; i++)
			for(j = 0; j < COLS; j++)
				if (hasLost)	// If player lost game
					screen[i][j] = gameOverScreen[j][i];
				else					// If player won game
					screen[i][j] = youWinScreen[j][i];
		drawScreen();		
	}
}
