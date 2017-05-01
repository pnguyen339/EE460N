.ORIG x1200

            ;SAVE REGISTERS ONTO STACK
            ADD R6, R6, #-2;
            STW R3, R6, #0;
            ADD R6, R6, #-2;
            STW R2, R6, #0;
            ADD R6, R6, #-2;
            STW R1, R6, #0;
            ADD R6, R6, #-2;
            STW R0, R6, #0;

            LEA R1, MASK
            LDW R1, R1, #0 ; R1 <- xFFFD
            LEA R2, START
            LDW R2, R2, #0 ; R2 <- x1000
            LEA R0, COUNTER
            LDW R0, R0, #0 ; R0 <- #128

LOOP        BRz DONE
            LDW R3, R2, #0 ; R3 <- PTE
            AND R3, R3, R1 ; CLEAR REFERENCE BIT
            STW R3, R2, #0 ;
            ADD R2, R2, #2 ; INCREMENT ADDRESS BY WORD
            ADD R0, R0, #-1; DECREMENT COUNTER
            BR LOOP

            ;RESTORE REGISTERS
DONE        LDW R0, R6, #0;
            ADD R6, R6, #2;
            LDW R1, R6, #0;
            ADD R6, R6, #2;
            LDW R2, R6, #0;
            ADD R6, R6, #2;
            LDW R3, R6, #0;
            ADD R6, R6, #2;

            RTI

USP         .FILL xFE00
MASK        .FILL xFFFD
START       .FILL x1000
COUNTER     .FILL x0080
            .END