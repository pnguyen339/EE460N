      .ORIG x3000

;ADD NEXT 20 BYTES
;STARTING AT xC000
      LEA R0, START
      LDW R0, R0, #0  ; ADDRESS
      ADD R1, R1, #10 ; COUNTER
      ADD R1, R1, #10

LOOP  BRZ DONE
      LDB R2, R0, #0
      ADD R4, R4, R2 ; TOTAL
      ADD R0, R0, #1 ; INCREMENT ADDRESS
      ADD R1, R1, #-1 ; DECREMENT COUNTER
      BR LOOP


DONE  STW R4, R0, #0
      JMP R4


INIT  .FILL x4000
START .FILL xC000
      .END