; test_branch — loops and conditional branches.
; Expected: R0=0x000A (loop runs 10 times).

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
    MOVL R1, #10        ; limit

.count_up:
    INC  R0
    CMP  R0, R1
    BNE  .count_up      ; loop until R0 == 10

    ; BEQ taken: R2 should become 1
    CLR  R2
    CMP  R0, R1
    COND EQ, 1
      INC R2            ; R2 = 1 (condition true)

    ; BLT not taken: R3 stays 0
    CLR  R3
    MOVL R4, #5
    CMP  R0, R4         ; 10 > 5, so LT is false
    COND LT, 1
      INC R3            ; should NOT execute

    ; Final: R0 = 10, R2 = 1, R3 = 0
    ; Combine: R0 = R0 * R2 + R3 = 10 * 1 + 0 = 10
    MUL  R0, R0, R2
    ADD  R0, R0, R3

    HALT                ; R0 = 0x000A
