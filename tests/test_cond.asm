; test_cond — COND predication prefix.
; Expected: R0=0x0007.

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
    ; COND EQ, 1 — condition true: executed
    MOVL R1, #5
    MOVL R2, #5
    CMP  R1, R2
    COND EQ, 1
      MOVL R0, #1           ; R0 = 1 (executed)

    ; COND NE, 1 — condition false: skipped
    COND NE, 1
      MOVL R0, #99          ; should NOT execute

    ; COND GT, 2 — R1=5 > R2=3: true, 2 instructions executed
    MOVL R2, #3
    CMP  R1, R2             ; 5 > 3 => GT true
    COND GT, 2
      ADDI R0, R0, #2       ; R0 = 3 (executed)
      ADDI R0, R0, #4       ; R0 = 7 (executed)

    ; COND LT, 2 — 5 < 3 is false: both skipped
    CMP  R1, R2             ; 5 > 3, so LT false
    COND LT, 2
      MOVL R0, #0           ; skipped
      MOVL R0, #0           ; skipped

    HALT                    ; R0 = 0x0007
