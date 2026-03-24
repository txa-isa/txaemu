// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>
//
// TXA16-1 private CPU state, SR bit definitions, and register-access macros.

#pragma once

#include <stdbool.h>
#include <stdint.h>

#define TXA16_1_MEM_SIZE  0x10000u
#define TXA16_1_SP_RESET  0xFFFEu

// SR bit masks
#define SR_Z    (1u << 0)
#define SR_C    (1u << 1)
#define SR_N    (1u << 2)
#define SR_OV   (1u << 3)
#define SR_SAT  (1u << 6)
#define SR_BANK (1u << 7)
#define SR_IE   (1u << 8)

// Interrupt vector table addresses
#define IVT_RESET 0x0000u
#define IVT_NMI   0x0002u
#define IVT_IRQ0  0x0004u   // IRQn at IVT_IRQ0 + n*2

typedef struct {
    uint16_t gpr[2][8];  // gpr[bank][reg], bank 0=User 1=IRQ
    uint16_t pc;
    uint16_t sp;
    uint16_t sr;

    int  cond_remaining; // predicated instructions left (0 = not predicated)
    bool cond_true;      // condition result from the last COND
} txa16_1_state_t;

// SR flag helpers — operate directly on the state struct.
#define GET_FLAG(s, f)     (((s)->sr & (uint16_t)(f)) != 0)
#define PUT_FLAG(s, f, v)  ((v) ? ((s)->sr |= (uint16_t)(f)) \
                                : ((s)->sr &= (uint16_t)~(uint16_t)(f)))

// Active bank index (0 or 1).
#define BANK(s)   (((s)->sr & SR_BANK) ? 1 : 0)

// Access a GPR in the currently active bank.
#define GPR(s, r) ((s)->gpr[BANK(s)][(r) & 7])
