; Iterative factorial: R0 = factorial(R0).
; Entry: R0 = n (call via CALL factorial).
; Exit:  R0 = n! (lower 16 bits).
; Demonstrates: PUSH/POP, CALL/RET, MUL, SUBI, CMP, BLE.

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
    MOVL R0, #6         ; compute 6! = 720 = 0x02D0
    CALL factorial
    HALT                ; R0 = 0x02D0 on exit

; factorial(R0) — iterative, clobbers R1 R2
factorial:
    PUSH R4
    MOV  R4, R0         ; R4 = n
    MOVL R0, #1         ; result = 1
    MOVL R1, #2         ; i = 2
.loop:
    CMP  R1, R4
    BGT  .done          ; if i > n, done
    MUL  R0, R0, R1     ; result *= i
    INC  R1             ; i++
    JMP  .loop
.done:
    POP  R4
    RET
