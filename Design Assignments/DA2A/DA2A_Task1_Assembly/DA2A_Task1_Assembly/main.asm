; DA2A_Task1_Assembly.asm
;
; Created: 3/2/2019 2:56:06 AM
; Author : John Duriman

.org 0
	SBI DDRB, 2				;Make Port B.2 an output port
L1: 
	CBI PORTB, 2			;Turn LED light on
	CALL Delay1				;Delay of .435 seconds which is 60% of .735
	
	SBI PORTB, 2			;Turn LED light off
	CALL Delay2				;Delay of .30 seconds which is 40% of .735

	RJMP L1					;Repeat Loop

;435ms at 16 MHz
Delay1:
	LDI R19, 99				;R19 = 99
Delay1a:
	LDI R20, 116			;R20 = 116
Delay1b:
	LDI R21, 200			;R21 = 200
Delay1c:
	DEC R21					;Decrement R21
	BRNE Delay1c			;If (R21 != 0) got to delay1c
	DEC R20					;Decrement R20
	BRNE Delay1b			;If (R20 != 0) got to delay1b
	DEC R19					;Decrement R19
	BRNE Delay1a			;If (R19 != 0) got to delay1a
	RET						;Return to caller


;300ms at 16 MHz
Delay2:
	LDI R19, 70				;R19 = 25
Delay2a:
	LDI R20, 116			;R20 = 194
Delay2b:
	LDI R21, 196			;R21 = 152
Delay2c:
	DEC R21					;Decrement R21
	BRNE Delay2c			;If (R21 != 0) got to delay2c
	DEC R20					;Decrement R20
	BRNE Delay2b			;If (R20 != 0) got to delay2b
	DEC R19					;Decrement R19
	BRNE Delay2a			;If (R19 != 0) got to delay2a
	nop
	RET						;Return to caller
	


