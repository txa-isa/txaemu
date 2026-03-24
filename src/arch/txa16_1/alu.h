// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>
//
// Arithmetic helpers shared by all execute modules.

#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "state.h"

// Set Z and N from a 16-bit result.
static inline void flags_zn(txa16_1_state_t *s, uint16_t r)
{
    PUT_FLAG(s, SR_Z, r == 0);
    PUT_FLAG(s, SR_N, (r >> 15) & 1);
}

// Set Z and N; clear C and OV (used by AND, OR, XOR, NOT, MUL, MULH).
static inline void flags_logic(txa16_1_state_t *s, uint16_t r)
{
    flags_zn(s, r);
    s->sr &= (uint16_t)~(uint16_t)(SR_C | SR_OV);
}

// Evaluate a condition code against the current SR flags.
// cc encoding matches Bcc / COND: 0=EQ 1=NE 2=GT 3=LT 4=GE 5=LE 6=CS 7=CC
static inline bool eval_cc(const txa16_1_state_t *s, uint8_t cc)
{
    bool z  = GET_FLAG(s, SR_Z);
    bool c  = GET_FLAG(s, SR_C);
    bool n  = GET_FLAG(s, SR_N);
    bool ov = GET_FLAG(s, SR_OV);
    switch (cc) {
    case 0: return z;
    case 1: return !z;
    case 2: return !z && (n == ov);
    case 3: return n != ov;
    case 4: return n == ov;
    case 5: return z || (n != ov);
    case 6: return c;
    case 7: return !c;
    }
    return false;
}

// Saturating addition — returns the 16-bit result and updates all flags.
uint16_t alu_add(txa16_1_state_t *s, uint16_t a, uint16_t b);

// Saturating subtraction (no-borrow carry convention) — updates all flags.
uint16_t alu_sub(txa16_1_state_t *s, uint16_t a, uint16_t b);
