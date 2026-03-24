// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>
//
// Opcode 0xE — CTL: NOP HALT SEI CLI SSAT CSAT
// Opcode 0xF — COND (predication prefix)

#include "emu/cpu.h"
#include "arch/txa16_1/state.h"
#include "arch/txa16_1/decode.h"
#include "arch/txa16_1/alu.h"
#include "arch/txa16_1/execute.h"

int exec_ctl_group(txa_cpu_t *cpu, txa16_1_state_t *s, uint16_t instr)
{
    uint8_t opcode = dec_opcode(instr);

    if (opcode == 0xF) { // COND predication prefix
        s->cond_true      = eval_cc(s, dec_cc(instr));
        s->cond_remaining = (int)dec_cnt(instr) + 1;
        return 0;
    }

    // opcode == 0xE: CTL
    uint8_t func = dec_cc(instr); // func sits in bits 11:9 = cc field
    switch (func) {
    case 0: break;                                            // NOP
    case 1: cpu->halted = true; break;                       // HALT
    case 2: s->sr |=  (uint16_t)SR_IE;  break;              // SEI
    case 3: s->sr &= (uint16_t)~(uint16_t)SR_IE;  break;   // CLI
    case 4: s->sr |=  (uint16_t)SR_SAT; break;              // SSAT
    case 5: s->sr &= (uint16_t)~(uint16_t)SR_SAT; break;   // CSAT
    default: return -1;
    }
    return 0;
}
