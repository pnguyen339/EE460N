
	AND R0, R0, #0		; result in R0

	AND R1, R1, #0
	ADD R1, R1, #1 	; loop counter in R1

	LEA R2, ADR, #0		; pointer in R2
	LDW R2, R2, #0

	LDB R3, R2, #0		; number
	ADD R2, R2, #1		; increment pointer

	AND R3, R3, #1		; increment if odd
	ADD R0, R0, R3

	ADD R1, R1, #-1

	STR R0 R2			; store in array once done

