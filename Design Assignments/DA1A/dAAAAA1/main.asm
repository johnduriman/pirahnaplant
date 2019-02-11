; dAAAAA1.asm
; Created: 2/8/2019 12:17:31 AM
; Author : John Duriman
;

;0x0499 x 0x099 = 0x02BF71
.include <m328pdef.inc>

.ORG 0
	LDI R25, 0x04	;Multiplicand
	LDI R24, 0x99	;Multiplicand
	LDI R22, 0x99	;Multiplier

multiply:			;Label to loop the decrementer and adder
	CPI R22, 0		;Compares Register to 0
	BREQ end		;If the register is 0, jump to end

	add R18, R24	;Adds the multiplicand to the final result
	adc R19, R25	;Adds the multiplicand to the final result plus the carry from the lower register
	adc R20, R0		;Adds the carry onto this register
	dec R22			;Decrements the multiplier
	rjmp multiply	;Jumps to the label multiply to repeat the process


end: rjmp end		;The end of the program, but here it jumps to itself so the registers don't get cleared



