; test_alu — basic ALU operations.
; Expected: R0=0x001D on HALT.
; 0x001D = 29 = ((5+3)*3) + (7 AND 0x0F) + NOT_flag_check

    .org 0x0000
    .dw _start
    .dw _abort
    .dw _abort
    .dw _abort
    .dw _abort
    .dw _abort
    .dw _abort
    .dw _abort
    .dw _abort
    .dw _abort

_abort:
    HALT

_start:
    ; ADD: R1 = 5+3 = 8
    MOVL R1, #5
    MOVL R2, #3
    ADD  R0, R1, R2         ; R0 = 8

    ; MUL: R0 = 8*3 = 24
    MOVL R3, #3
    MUL  R0, R0, R3         ; R0 = 24 = 0x18

    ; SUB: R4 = 10 - 3 = 7, then AND with 0x0F = 7
    MOVL R4, #10
    SUB  R4, R4, R3         ; R4 = 7
    ANDI R4, R4, #0x0F      ; R4 = 7 (no change, just tests ANDI)

    ; NEG: R5 = -7, then ADD R0 = 24 + (-7) = 17
    ; but NEG of 7 = 0xFFF9 which is negative.
    ; Instead use SUBI: R0 = 24 - 7 = 17
    SUBI R0, R0, #7         ; R0 = 17 = 0x11

    ; ORI: R0 = 17 | 8 = 25 = 0x19
    ORI  R0, R0, #8         ; R0 = 25

    ; XOR with self clears: R6 = 0
    MOVL R6, #0xFF
    XOR  R6, R6, R6
    ; ADD zero: R0 unchanged
    ADD  R0, R0, R6         ; R0 = 25

    ; CMP + conditional: if R0 == 25 then R0 += 4 => 29
    MOVL R7, #25
    CMP  R0, R7
    COND EQ, 1
      ADDI R0, R0, #4       ; R0 = 29 = 0x001D

    HALT
