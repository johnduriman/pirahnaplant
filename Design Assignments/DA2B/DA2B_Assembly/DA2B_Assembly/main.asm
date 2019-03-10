; DA2B_Assembly.asm
;
; Created: 3/8/2019 8:25:45 PM
; Author : John Duriman
.include <m328pdef.inc>

.org 0
	JMP MAIN
.org 0x02					;Location for external interrupt 0
	JMP EX0_ISR

MAIN: 
	SBI DDRB, 2				;Make Port B.2 an output port
	SBI PORTB, 2			;Turn LED light off
	SBI PORTD, 2			;Pull-up activated
	LDI R20, 0x2			;Make INT0 falling edge triggered
	STS EICRA, R20

	//Stack setup
	LDI R20, HIGH(RAMEND)
	OUT SPH, R20
	LDI R20, LOW(RAMEND)
	OUT SPL, R20			
	
	LDI R20, 1<<INT0		;Enable INT0
	OUT EIMSK, R20
	SEI						;Enable interrupts
	RJMP MAIN				;Repeat Loop


EX0_ISR:	CBI PORTB, 2			;Turn LED light on
			;1.25s at 16 MHz
Delay1:		LDI R19, 200			;R19 = 102
Delay1a:	LDI R20, 118			;R20 = 118
Delay1b:	LDI R21, 194			;R21 = 194
Delay1c:	DEC R21					;Decrement R21
			BRNE Delay1c			;If (R21 != 0) got to delay1c
			DEC R20					;Decrement R20
			BRNE Delay1b			;If (R20 != 0) got to delay1b
			DEC R19					;Decrement R19
			BRNE Delay1a			;If (R19 != 0) got to delay1a
			RETI
	


	