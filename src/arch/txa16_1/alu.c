// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>

#include "alu.h"

uint16_t alu_add(txa16_1_state_t *s, uint16_t a, uint16_t b)
{
    uint32_t r32   = (uint32_t)a + (uint32_t)b;
    uint16_t r     = (uint16_t)r32;
    bool     carry = r32 > 0xFFFFu;
    bool     ov    = (!(a & 0x8000) && !(b & 0x8000) &&  (r & 0x8000))
                  || ( (a & 0x8000) &&  (b & 0x8000) && !(r & 0x8000));

    if (GET_FLAG(s, SR_SAT) && ov)
        r = (a & 0x8000) ? 0x8000u : 0x7FFFu;

    flags_zn(s, r);
    PUT_FLAG(s, SR_C,  carry);
    PUT_FLAG(s, SR_OV, ov);
    return r;
}

uint16_t alu_sub(txa16_1_state_t *s, uint16_t a, uint16_t b)
{
    uint32_t r32   = (uint32_t)a - (uint32_t)b;
    uint16_t r     = (uint16_t)r32;
    bool     carry = (a >= b);  // no-borrow: C=1 means no borrow
    bool     ov    = (!(a & 0x8000) &&  (b & 0x8000) &&  (r & 0x8000))
                  || ( (a & 0x8000) && !(b & 0x8000) && !(r & 0x8000));

    if (GET_FLAG(s, SR_SAT) && ov)
        r = (a & 0x8000) ? 0x8000u : 0x7FFFu;

    flags_zn(s, r);
    PUT_FLAG(s, SR_C,  carry);
    PUT_FLAG(s, SR_OV, ov);
    return r;
}
