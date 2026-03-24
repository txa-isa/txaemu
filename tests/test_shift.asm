; test_shift — shift and rotate operations.
; Expected: R0=0x0001.

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
    ; SHL: 1 << 8 = 256 = 0x0100
    MOVL R1, #1
    SHL  R0, R1, #8         ; R0 = 0x0100

    ; SHR: 0x0100 >> 4 = 0x0010
    SHR  R0, R0, #4         ; R0 = 0x0010

    ; SAR: 0x0010 >> 2 = 0x0004 (positive, same as SHR)
    SAR  R0, R0, #2         ; R0 = 0x0004

    ; SHR: 0x0004 >> 1 = 0x0002
    SHR  R0, R0, #1         ; R0 = 0x0002

    ; ROL: 0x0002 rotated left 15 = 0x0001 (bit 1 wraps to bit 0 via bit 15)
    ROL  R0, R0, #15        ; R0 = 0x0001

    ; Verify: SAR on negative value
    MOVL R2, #0xFF
    MOVH R2, #0xFF          ; R2 = 0xFFFF = -1
    SAR  R2, R2, #4         ; R2 = 0xFFFF (arithmetic, sign preserved)
    MOVL R3, #0xFF
    MOVH R3, #0xFF
    CMP  R2, R3             ; should be equal
    COND NE, 1
      CLR R0                ; fail: clear R0

    HALT                    ; R0 = 0x0001 on success
