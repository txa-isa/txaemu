// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>
//
// Opcode 0x9 — SHIFT: SHL SHR SAR ROL

#include "emu/cpu.h"
#include "arch/txa16_1/state.h"
#include "arch/txa16_1/decode.h"
#include "arch/txa16_1/alu.h"
#include "arch/txa16_1/execute.h"

int exec_shift_group(txa_cpu_t *cpu, txa16_1_state_t *s, uint16_t instr)
{
    (void)cpu;
    uint8_t  rd     = dec_rd(instr);
    uint8_t  rs     = dec_rs1(instr);
    uint8_t  type   = dec_shift_type(instr);
    uint8_t  amount = dec_shift_amount(instr);
    uint16_t val    = GPR(s, rs);
    uint16_t result;
    bool     new_c  = GET_FLAG(s, SR_C); // unchanged when amount == 0

    if (amount == 0) {
        result = val;
    } else {
        switch (type) {
        case 0: // SHL
            new_c  = (val >> (16 - amount)) & 1;
            result = (uint16_t)(val << amount);
            break;
        case 1: // SHR
            new_c  = (val >> (amount - 1)) & 1;
            result = (uint16_t)(val >> amount);
            break;
        case 2: // SAR
            new_c  = (val >> (amount - 1)) & 1;
            result = (uint16_t)((int16_t)val >> amount);
            break;
        case 3: // ROL
            new_c  = (val >> (16 - amount)) & 1;
            result = (uint16_t)((val << amount) | (val >> (16 - amount)));
            break;
        default:
            return -1;
        }
    }

    flags_zn(s, result);
    PUT_FLAG(s, SR_C, new_c);
    s->sr &= (uint16_t)~(uint16_t)SR_OV;
    GPR(s, rd) = result;
    return 0;
}
