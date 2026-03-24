; test_stack — PUSH/POP, CALL/RET, stack frame preservation.
; Expected: R0=0x0078 (120 = 5! via iterative factorial).

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
    MOVL R0, #5
    CALL factorial      ; R0 = 5! = 120 = 0x78
    HALT

; factorial(R0) -> R0 = R0!  iterative, callee-saves R4
factorial:
    PUSH R4
    MOV  R4, R0         ; R4 = n
    MOVL R0, #1         ; result = 1
    MOVL R1, #2         ; i = 2
.loop:
    CMP  R1, R4
    BGT  .done
    MUL  R0, R0, R1
    INC  R1
    JMP  .loop
.done:
    POP  R4
    RET
