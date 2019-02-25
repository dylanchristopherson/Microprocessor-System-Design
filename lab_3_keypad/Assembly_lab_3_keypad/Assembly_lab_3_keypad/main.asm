;
; Interfacing with a 4 by 4 keypad and 8 leds with the atmega8515 microcontrollers
;
; 4 by 4 keypad is connected to PORTD
; LEDs are connected to PORTB
;

.def temp = r16

.include "M8515DEF.INC"

.def key = r17
.def check = r24
.def xtime = r25
.cseg
.org 0

rjmp start

;The "start" Routine is to create stack and set output port with PORTB

start:				; create a stack to store return addresses
    ldi temp, low(RAMEND)	;load SPL (the low byte of the stack) 
    out SPL, temp			;load low byte address to SPL pointer register
	ldi temp, high(RAMEND)	;load SPH (the high byte of the stack)
	out SPH, temp			;load high byte address to SPH pointer register

	ldi temp, 0xFF
	out DDRB, temp
	out PORTB, temp

main:
	rcall get_key	; the get_key function call is to dead the pressed keys from a 4x4 keypad
	mov xtime, key	; store the pressed key r17 to register r25
	rcall f1		; function call to flash LEDs xtime on the PORTB connected to the LEDs
	rjmp main       ; jump back to the main routine

scan:
	ldi temp, 0xF0	; load 0xF0 to temp
					; -the higher byte of (0xF0) is connected to keypad outputs
					; -and the lower byte of (0xF0) is connected to keypad input
	out DDRD, temp  
	ldi check, 0x0F ; load 0x0F to a register "check"
	ldi temp, 0x0F  ; load 0x0F to temp; this assign a high signal to a keypad input
	out PORTD, temp 

not_ready:
	in temp,PIND	   ;
	eor temp, check    ;Why is xor, eor?
	brne not_ready	   ;conditional branch

scan_loop:
	ldi temp, 0xFF     ; load 0xFF to the temp register
	ldi xtime, 0x04    ; load 0x04 to xtime register to scan rows and columns
	clc				   ; clear carry flag

scan_next_row:
	ror temp			; rotate to the right for the temp register
	mov check, temp     ; move the temp register to check
	out PORTD, temp     ; output temp to PORTD
	in key, PIND        ; input PIND to key
	eor key, temp        ; compare the values in the temp register and the key register
	breq next_key        
	rcall sdelay		     ; call delay function second delay for keypad
	in key, PIND			 ; input PIND to the key register
	eor temp, key            ; xor temp with the pressed key
	brne gotkey

; brne: branch not equal
; breq: branch equal
; xor: eor

next_key:
	mov temp, check		; move check register to temp register
	dec xtime			; decrement xtime by 1 (xtime-1)
	brne scan_next_row  ; if temp is not zero, jump to scan_next_row
	rjmp scan_loop

gotkey:
	ret	     ; return to the main routine when a key match occurs

get_key:
	rcall scan				  ; call the function call scan to scan for any pressed keys
	ldi xtime, 0x00           ; load 0x00 to xtime register
	ldi ZL, LOW(2*key_code)   ; load the key code to the lower byte of Z register
	ldi ZH, HIGH(2*key_code)  ; load the key code to the higher byte of Z register

check_next:

	lpm			; use the instruction LPM to load the Z register pointer to the adderss

	mov temp, r0	; move the r0 to the temp register
	cp temp, key	; compare (cp) temp with the pressed key
	breq found	    ; branch to found routine if temp is zero
	adiw ZH:ZL, 1   ; increment the Z register pointer to the next position
	inc xtime       ; increment the xtime by 1
	cpi xtime, 0x10  ; compare (cpi) with 0x10
	brne check_next  ; branch to check_next routine if xtime is not equal to zero
	ldi key, 0x20    ; load 0x20 to key if temp is equal to zero
	ret

found:
	mov key, xtime	; if the pressed key is found, move xtime to key register
	ret				; return to the main routine

; f1 routine is the routine to flash LEDs the number of times according to the number in
; (0-9) and (A-F) key is pressed on teh keypad. If the key "A" in hex is pressed,
; the LEDs flash 10 times	

f1: 
	ldi temp,0x00
	out PORTB,temp
	rcall one_sec_delay
	ldi temp,0xff
	out PORTB, temp
	rcall one_sec_delay

	cpi xtime, 0
	brne f2
	inc xtime

f2:
	dec xtime
	brne f1
	ret

;;;; Time delay for a keypad between each press ;;;;

sdelay: 
	ldi r21, 64
	ldi r22, 255

idelay: 
	dec r22
	brne idelay
	dec r21
	brne idelay
	ret

;;;; Time delay for LEDs flash ;;;;

one_sec_delay:

	ldi r20, 2
	ldi r21, 255
	ldi r22, 255

delay:
	dec r22
	brne delay
	dec r21
	brne delay
	dec r20
	brne delay
	ret

key_code:
	.db 0xE7, 0xEE, 0xDE, 0xBE, 0xED, 0xDD, 0xBD, 0xEB
	.db 0xDB, 0xBB, 0x7E, 0x7D, 0x7B, 0x77, 0xB7, 0xD7

; END	
		