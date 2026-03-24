// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>
//
// Opcode 0x0 — ALU 3-operand (ADD SUB AND OR XOR MUL MULH CMP)
// Opcode 0x1 — ALU2 2-operand (MOV NEG NOT LOADB STOREB)

#include "emu/cpu.h"
#include "arch/txa16_1/state.h"
#include "arch/txa16_1/decode.h"
#include "arch/txa16_1/alu.h"
#include "arch/txa16_1/execute.h"

int exec_alu_group(txa_cpu_t *cpu, txa16_1_state_t *s, uint16_t instr)
{
    uint8_t opcode = dec_opcode(instr);
    uint8_t rd     = dec_rd(instr);
    uint8_t rs1    = dec_rs1(instr);
    uint8_t rs2    = dec_rs2(instr);
    uint8_t func   = dec_func(instr);

    if (opcode == 0x0) {
        uint16_t a = GPR(s, rs1), b = GPR(s, rs2);
        switch (func) {
        case 0: GPR(s, rd) = alu_add(s, a, b); break;
        case 1: GPR(s, rd) = alu_sub(s, a, b); break;
        case 2: { uint16_t r = a & b; flags_logic(s, r); GPR(s, rd) = r; break; }
        case 3: { uint16_t r = a | b; flags_logic(s, r); GPR(s, rd) = r; break; }
        case 4: { uint16_t r = a ^ b; flags_logic(s, r); GPR(s, rd) = r; break; }
        case 5: { // MUL — lower 16 bits of unsigned product
            uint16_t r = (uint16_t)((uint32_t)a * (uint32_t)b);
            flags_logic(s, r);
            GPR(s, rd) = r;
            break;
        }
        case 6: { // MULH — upper 16 bits of signed product
            uint16_t r = (uint16_t)((uint32_t)((int32_t)(int16_t)a
                                              * (int32_t)(int16_t)b) >> 16);
            flags_logic(s, r);
            GPR(s, rd) = r;
            break;
        }
        case 7: alu_sub(s, GPR(s, rs1), GPR(s, rs2)); break; // CMP: flags only
        default: return -1;
        }
        return 0;
    }

    // opcode == 0x1: ALU2
    switch (func) {
    case 0: GPR(s, rd) = GPR(s, rs1); break; // MOV
    case 1: GPR(s, rd) = alu_sub(s, 0, GPR(s, rs1)); break; // NEG
    case 2: { uint16_t r = ~GPR(s, rs1); flags_logic(s, r); GPR(s, rd) = r; break; } // NOT
    case 3: GPR(s, rd) = mem_rb(cpu, GPR(s, rs1)); break; // LOADB
    case 4: mem_wb(cpu, GPR(s, rs1), (uint8_t)GPR(s, rs2)); break;  // STOREB
    default: return -1;
    }
    return 0;
}
