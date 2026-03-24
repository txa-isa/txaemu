; Word-aligned memory copy and verification.
; Copies 4 words from src to dst, then sums the copy to verify.
; Demonstrates: LDW/STW with offsets, loops, CALL/RET, ADD.

.equ SRC_ADDR, 0x0080
.equ DST_ADDR, 0x0090
.equ N_WORDS,  4

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
    ; Write test data to src: 1, 2, 3, 4
    MOVL R0, #SRC_ADDR
    MOVL R1, #1
    STW  R1, [R0, #0]
    MOVL R1, #2
    STW  R1, [R0, #1]
    MOVL R1, #3
    STW  R1, [R0, #2]
    MOVL R1, #4
    STW  R1, [R0, #3]

    ; memcpy(src, dst, N_WORDS)
    MOVL R0, #SRC_ADDR
    MOVL R1, #DST_ADDR
    MOVL R2, #N_WORDS
    CALL memcpy_w

    ; Sum the destination: R0 = 1+2+3+4 = 10
    MOVL R0, #DST_ADDR
    MOVL R2, #N_WORDS
    CALL array_sum      ; result in R3

    MOV  R0, R3         ; expose sum in R0
    HALT                ; R0 = 0x000A (10)

; memcpy_w(R0=src, R1=dst, R2=n_words)  clobbers R3
memcpy_w:
    AND  R2, R2, R2
    BEQ  .mcpy_done
.mcpy_loop:
    LOAD R3, [R0]
    STORE [R1], R3
    ADDI R0, R0, #2
    ADDI R1, R1, #2
    DEC  R2
    BNE  .mcpy_loop
.mcpy_done:
    RET

; array_sum(R0=ptr, R2=n)  -> R3 = sum  clobbers R4
array_sum:
    CLR  R3
.sum_loop:
    AND  R2, R2, R2
    BEQ  .sum_done
    LOAD R4, [R0]
    ADD  R3, R3, R4
    ADDI R0, R0, #2
    DEC  R2
    JMP  .sum_loop
.sum_done:
    RET
