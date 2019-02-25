;
; assembly_lab_2.asm
;
; Created: 2/6/2019 2:20:13 PM
; Author : Student
;


; Replace with your application code

;;;;;;;Knight Rider;;;;;;;
;/*
.include "m8515def.inc"
.def temp = r16

start:
    ldi temp, low(RAMEND)
	out SPL, temp
	ldi temp, HIGH(RAMEND)
	out SPH, temp

	ldi temp $ff
	out DDRB, temp
	out PORTB, temp

loadbyte:

	;1
	rol temp
	out PORTB, temp
	rcall one_sec_delay

	rol temp
	out PORTB, temp
	rcall one_sec_delay

	rol temp
	out PORTB, temp
	rcall one_sec_delay

	rol temp
	out PORTB, temp
	rcall one_sec_delay
	;5
	rol temp
	out PORTB, temp
	rcall one_sec_delay

	rol temp
	out PORTB, temp
	rcall one_sec_delay

	rol temp
	out PORTB, temp
	rcall one_sec_delay
	;8
	rol temp
	out PORTB, temp
	rcall one_sec_delay
	;1
	ror temp
	out PORTB, temp
	rcall one_sec_delay

	ror temp
	out PORTB, temp
	rcall one_sec_delay

	ror temp
	out PORTB, temp
	rcall one_sec_delay

	ror temp
	out PORTB, temp
	rcall one_sec_delay
	;5
	ror temp
	out PORTB, temp
	rcall one_sec_delay

	ror temp
	out PORTB, temp
	rcall one_sec_delay

	ror temp
	out PORTB, temp
	rcall one_sec_delay
	;8
	ror temp
	out PORTB, temp
	rcall one_sec_delay



	rjmp loadbyte

once_sec_delay:

	ldi r20, 20
	ldi r21, 25
	ldi r22, 25

delay:

    dec r22
	brne delay

	dec r21
	brne delay
	dec r20
	brne delay

	ret

/*





left:
	rol temp
	out PORTB, temp
	rcall one_sec_delay

right:
	ror temp
	out PORTB, temp
	rcall one_sec_delay


*/

;*/




;;;;;;;TRAFFIC LIGHT CONTROLLER;;;;;;;
/*

.include "m8515def.inc"
.def temp = r16

start:
    ldi temp, low(RAMEND)
	out SPL, temp
	ldi temp, HIGH(RAMEND)
	out SPH, temp

	ldi temp $ff
	out DDRB, temp
	out PORTB, temp

loadbyte:

	rol temp
	out PORTB, temp
	rcall one_sec_delay

	rol temp
	out PORTB, temp
	rcall one_sec_delay

	rol temp
	out PORTB, temp
	rcall one_sec_delay

	rol temp
	out PORTB, temp
	rcall one_sec_delay

	rol temp
	out PORTB, temp
	rcall one_sec_delay

	rol temp
	out PORTB, temp
	rcall one_sec_delay

	rol temp
	out PORTB, temp
	rcall one_sec_delay

	rol temp
	out PORTB, temp
	rcall one_sec_delay

	ror temp
	out PORTB, temp
	rcall one_sec_delay

	ror temp
	out PORTB, temp
	rcall one_sec_delay

	ror temp
	out PORTB, temp
	rcall one_sec_delay

	ror temp
	out PORTB, temp
	rcall one_sec_delay

	ror temp
	out PORTB, temp
	rcall one_sec_delay

	ror temp
	out PORTB, temp
	rcall one_sec_delay

	ror temp
	out PORTB, temp
	rcall one_sec_delay

	ror temp
	out PORTB, temp
	rcall one_sec_delay



	rjmp loadbyte

once_sec_delay:

	ldi r20, 20
	ldi r21, 25
	ldi r22, 25

delay:

    dec r22
	brne delay

	dec r21
	brne delay
	dec r20
	brne delay

	ret
*/