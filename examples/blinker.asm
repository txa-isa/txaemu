; Blinker example — toggle a bit in a memory-mapped I/O port with a delay loop.
; Demonstrates: MOVL/MOVH, LOADB/STOREB, XORI, SUBI, Bcc, JMP.

.equ IO_PORT,     0xFF00
.equ DELAY_COUNT, 1000

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
    MOVL R0, #0x00
    MOVH R0, #0xFF          ; R0 = 0xFF00 (IO_PORT)

.toggle:
    LOADB R1, [R0]          ; read port
    XORI  R1, R1, #0x01     ; toggle bit 0
    STOREB [R0], R1         ; write back

    MOVL  R2, #0xE8
    MOVH  R2, #0x03         ; R2 = 1000 (DELAY_COUNT)
.delay:
    SUBI  R2, R2, #1
    BNE   .delay

    JMP   .toggle
