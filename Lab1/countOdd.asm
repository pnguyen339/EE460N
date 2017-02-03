	.ORIG x3000
	AND R0, R0, #0		; result

	AND R1, R1, #0
	ADD R1, R1, #256	; loop counter

	AND R2, R2, #0
	ADD R2, R2, #4
	LSL R2, R2, #12		; pointer

LOOP
	LDR R3, R2, #0		; number
	ADD R2, R2, #2		; increment pointer

	AND R3, R3, #1		; increment if odd
	ADD R0, R0, R3

	ADD R1, R1, #-1
	BRp LOOP

	STR R0 R2			; store in array

	.END
