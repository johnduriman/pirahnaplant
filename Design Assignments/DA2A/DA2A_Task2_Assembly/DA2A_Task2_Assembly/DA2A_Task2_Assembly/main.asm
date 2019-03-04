; DA2A_Task2_Assembly.asm
;
; Created: 3/2/2019 2:56:06 AM
; Author : John Duriman

.org 0
	SBI DDRB, 2				;Make Port B.2 an output port
	CBI DDRC, 1				;Make Port C.1 as an input port
L1: 
	SBI PORTB, 2			;Turn LED light off
	SBIS PINC, 1			;Check if PORTC.1 is LOW, then skip next instruction
	CALL LEDON				;Apply Delay and turn the light on

	RJMP L1					;Repeat Loop

LEDON:
	CBI PORTB, 2			;Turn LED light on
;1.25s at 16 MHz
Delay1:
	LDI R19, 200			;R19 = 102
Delay1a:
	LDI R20, 171			;R20 = 118
Delay1b:
	LDI R21, 194			;R21 = 194
Delay1c:
	DEC R21					;Decrement R21
	BRNE Delay1c			;If (R21 != 0) got to delay1c
	DEC R20					;Decrement R20
	BRNE Delay1b			;If (R20 != 0) got to delay1b
	DEC R19					;Decrement R19
	BRNE Delay1a			;If (R19 != 0) got to delay1a
	RET						;Return to caller



