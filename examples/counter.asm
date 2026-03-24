; Counter — count from 0 to 9 (inclusive), then HALT.
; R0 holds the counter.  Demonstrates: CLR, INC, CMP, COND, JMP, HALT.

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
    CLR  R0             ; counter = 0
    MOVL R1, #10        ; limit = 10
.loop:
    INC  R0
    CMP  R0, R1
    BNE  .loop          ; repeat while counter != 10
    HALT                ; R0 = 10 on exit
