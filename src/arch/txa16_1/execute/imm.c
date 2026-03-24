// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>
//
// Opcodes 0x2-0x6 — immediate ALU: ADDI SUBI ANDI ORI XORI

#include "emu/cpu.h"
#include "arch/txa16_1/state.h"
#include "arch/txa16_1/decode.h"
#include "arch/txa16_1/alu.h"
#include "arch/txa16_1/execute.h"

int exec_imm_group(txa_cpu_t *cpu, txa16_1_state_t *s, uint16_t instr)
{
    (void)cpu;
    uint8_t  opcode = dec_opcode(instr);
    uint8_t  rd     = dec_rd(instr);
    uint8_t  rs     = dec_rs1(instr);
    uint16_t imm    = dec_imm6(instr);

    switch (opcode) {
    case 0x2: GPR(s, rd) = alu_add(s, GPR(s, rs), imm); break; // ADDI
    case 0x3: GPR(s, rd) = alu_sub(s, GPR(s, rs), imm); break; // SUBI
    case 0x4: { uint16_t r = GPR(s, rs) & imm; flags_logic(s, r); GPR(s, rd) = r; break; }
    case 0x5: { uint16_t r = GPR(s, rs) | imm; flags_logic(s, r); GPR(s, rd) = r; break; }
    case 0x6: { uint16_t r = GPR(s, rs) ^ imm; flags_logic(s, r); GPR(s, rd) = r; break; }
    default:  return -1;
    }
    return 0;
}
