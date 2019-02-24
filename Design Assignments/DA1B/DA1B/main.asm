; DA1B.asm
; Created: 2/20/2019 10:37:43 PM
; Author : John Duriman
;

;Sum of 101 + 102 + ... + 198 + 199 = 14,850 = 0x3A02
;Sum of # divisible by 3: 102 + 105 + ... + 198 = 4950 = 0x1356
;Sum of # not divisible by 3: 101 + 103 + ... + 199 = 9900 = 0x26AC 

.include <m328pdef.inc>

.SET STARTADDS = 0x0200
.SET MULT3 = 0x0400
.SET NOTMULT3 = 0x0600

.ORG 0
	LDI R20, 99						;R20 = 99 (Counter)
	LDI R21, 101					;R21 = 101 (Number between 10 & 255)

	LDI XH, HIGH(STARTADDS)			;R27 = $02
	LDI XL, LOW(STARTADDS)			;R26 = $00

	LDI YH, HIGH(MULT3)				;R29 = $04
	LDI YL, LOW(MULT3)				;R28 = $00

	LDI ZH, HIGH(NOTMULT3)			;R31 = $06
	LDI ZL, LOW(NOTMULT3)			;R30 = $00


L1:	
	ST X, R21						;Copy R21(101) to memory $0200
	MOV R22, R21					;Copy R21 to R22	

MOD3:								;Uses R22 as a temp to check modulus 3
	SUBI R22, 3						;Subtract R22 by 3

	CPI R22, 2						;Check for remainder 2
	BREQ STORETO6					;Not a multiple of 3, store at 0x0600
	CPI R22, 1						;Check for remainder 1
	BREQ STORETO6					;Not a multiple of 3, store at 0x0600
	CPI R22, 0						;Check for no remainder
	BREQ STORETO4					;Is a multiple of 3, store at 0x0400

	JMP MOD3						;Loop to MOD3 since R22 > 2

STORETO6:
	ST Z, R21						;Stores R21 to memory $0600
	add R18, R21					;R18 = R18 + R21
	adc R19, R0						;R19 = R19 + carry
	INC ZL							;Increment low byte of Z
	JMP INCREMENT					;Jump to Increment

STORETO4:
	ST Y, R21						;Stores R21 to memory $0400
	add R16, R21					;R16 = R16 + R21
	adc R17, R0						;R17 = R17 + carry
	INC YL							;Increment low byte of Y

INCREMENT:
	INC XL							;Increment low byte of X
	INC R21							;Increment chosen consecutive numbers
	DEC R20							;Decrement the counter
	BRNE L1							;Loop until counter = zero

END: 
	JMP END							;Loop infinitely here