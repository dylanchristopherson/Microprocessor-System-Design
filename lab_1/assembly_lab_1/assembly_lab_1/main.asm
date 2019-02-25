;
; AssemblerApplication1.asm
;
; Created: 1/28/2019 3:29:12 PM
; Author : Student
;


; Replace with your application code

.include "m8515def.inc"			; Header file for ATmega8515 micro
.def temp = r16					; rename r16 to temp variable

start:

    ldi temp, low(RAMEND)	;load SPL (the low byte of the stack) 
    out SPL, temp			;load low byte address to SPL pointer register
	ldi temp, high(RAMEND)	;load SPH (the high byte of the stack)
	out SPH, temp			;load high byte address to SPH pointer register

	ldi temp, $ff		;Set up PORTB as outputs
	out DDRB, temp			;DDRB means "Data register B"
	out PORTB, temp	

loadbyte:


	ldi temp, $aa
	out PORTB, temp
	rcall one_sec_delay
	ldi temp, $55
	out PORTB, temp
	rcall one_sec_delay
	rjmp loadbyte

one_sec_delay:

	ldi r20, 20				;20d = 14h
	ldi r21, 25				;25d = 19h
	ldi r22, 25

delay:
							;delays wit a nested loop
	dec r22					;255*255 total iterations
	brne delay

	dec r21					;255 iterations
	brne delay

	dec r20					;20 iterations
	brne delay

	ret						;return

	
	;ldi temp, $aa
	;out PORTB, temp
	;rcall one_sec_delay
	;ldi temp, $55
	;out PORTB, temp
	;rcall one_sec_delay
	;rjmp loadbyte
	
	;rol temp				;roll the bits
	;out PORTB, temp			;update LEDs
	;rcall one_sec_delay		;call the one_sec_delay
	;rjmp loadbyte			;repeat
