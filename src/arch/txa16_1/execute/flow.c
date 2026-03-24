// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>
//
// Opcode 0xB — Bcc (conditional branch)
// Opcode 0xC — JMP / CALL (PC-relative)
// Opcode 0xD — SYS: PUSH POP JMP.R CALL.R RDSR WRSR RET RETI

#include "emu/cpu.h"
#include "arch/txa16_1/state.h"
#include "arch/txa16_1/decode.h"
#include "arch/txa16_1/alu.h"
#include "arch/txa16_1/execute.h"

int exec_flow_group(txa_cpu_t *cpu, txa16_1_state_t *s, uint16_t instr)
{
    uint8_t opcode = dec_opcode(instr);

    switch (opcode) {
    case 0xB: { // Bcc
        uint8_t cc  = dec_cc(instr);
        int32_t off = sign_ext(dec_off9(instr), 9);
        if (eval_cc(s, cc))
            s->pc = (uint16_t)(s->pc + off * 2);
        break;
    }
    case 0xC: { // JMP / CALL
        int32_t  off    = sign_ext(dec_off11(instr), 11);
        uint16_t target = (uint16_t)(s->pc + off * 2);
        if (dec_t(instr)) { // CALL: push return address
            s->sp -= 2;
            mem_ww(cpu, s->sp, s->pc);
        }
        s->pc = target;
        break;
    }
    case 0xD: { // SYS
        uint8_t rd   = dec_rd(instr);
        uint8_t func = dec_rs1(instr); // func sits in bits 8:6 = rs1 field
        switch (func) {
        case 0: s->sp -= 2; mem_ww(cpu, s->sp, GPR(s, rd)); break;     // PUSH
        case 1: GPR(s, rd) = mem_rw(cpu, s->sp); s->sp += 2; break;    // POP
        case 2: s->pc = GPR(s, rd); break;                              // JMP.R
        case 3: s->sp -= 2; mem_ww(cpu, s->sp, s->pc);                 // CALL.R
                s->pc = GPR(s, rd); break;
        case 4: GPR(s, rd) = s->sr; break;                             // RDSR
        case 5: s->sr = GPR(s, rd); break;                             // WRSR
        case 6: s->pc  = mem_rw(cpu, s->sp); s->sp += 2; break;       // RET
        case 7: s->sr  = mem_rw(cpu, s->sp); s->sp += 2;              // RETI
                s->pc  = mem_rw(cpu, s->sp); s->sp += 2; break;
        default: return -1;
        }
        break;
    }
    default:
        return -1;
    }
    return 0;
}
