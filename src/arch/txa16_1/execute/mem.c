// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>
//
// Opcode 0x7 — LDW
// Opcode 0x8 — STW
// Opcode 0xA — MOVL / MOVH

#include "emu/cpu.h"
#include "arch/txa16_1/state.h"
#include "arch/txa16_1/decode.h"
#include "arch/txa16_1/execute.h"

int exec_mem_group(txa_cpu_t *cpu, txa16_1_state_t *s, uint16_t instr)
{
    uint8_t opcode = dec_opcode(instr);
    uint8_t rd     = dec_rd(instr);
    uint8_t rs     = dec_rs1(instr);

    switch (opcode) {
    case 0x7: { // LDW
        int32_t  off = sign_ext(dec_imm6(instr), 6);
        uint16_t ea  = (uint16_t)(GPR(s, rs) + off * 2);
        GPR(s, rd) = mem_rw(cpu, ea);
        break;
    }
    case 0x8: { // STW
        int32_t  off = sign_ext(dec_imm6(instr), 6);
        uint16_t ea  = (uint16_t)(GPR(s, rs) + off * 2);
        mem_ww(cpu, ea, GPR(s, rd));
        break;
    }
    case 0xA: { // MOVL / MOVH
        uint8_t h   = dec_h(instr);
        uint8_t imm = dec_imm8(instr);
        if (h == 0)
            GPR(s, rd) = imm;
        else
            GPR(s, rd) = (GPR(s, rd) & 0x00FFu) | ((uint16_t)imm << 8);
        break;
    }
    default:
        return -1;
    }
    return 0;
}
