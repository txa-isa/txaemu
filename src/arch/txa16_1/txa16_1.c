// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>
//
// Top-level TXA16-1 glue: execute() dispatch and architecture descriptor.

#include "emu/cpu.h"
#include "state.h"
#include "decode.h"
#include "execute.h"
#include "txa16_1.h"

// Forward declarations for functions defined in separate files.
void txa16_1_reset(txa_cpu_t *cpu);
int  txa16_1_step (txa_cpu_t *cpu);
void txa16_1_dump (txa_cpu_t *cpu);

// Dispatch one decoded instruction to the appropriate execute group.
int execute(txa_cpu_t *cpu, txa16_1_state_t *s, uint16_t instr)
{
    switch (dec_opcode(instr)) {
    case 0x0:
    case 0x1:                       return exec_alu_group  (cpu, s, instr);
    case 0x2: case 0x3: case 0x4:
    case 0x5: case 0x6:             return exec_imm_group  (cpu, s, instr);
    case 0x7: case 0x8: case 0xA:   return exec_mem_group  (cpu, s, instr);
    case 0x9:                        return exec_shift_group(cpu, s, instr);
    case 0xB: case 0xC: case 0xD:   return exec_flow_group (cpu, s, instr);
    case 0xE: case 0xF:             return exec_ctl_group  (cpu, s, instr);
    default:                         return -1;
    }
}

const txa_arch_t txa16_1_arch = {
    .name       = "txa16-1",
    .word_bits  = 16,
    .mem_size   = TXA16_1_MEM_SIZE,
    .state_size = sizeof(txa16_1_state_t),
    .reset      = txa16_1_reset,
    .step       = txa16_1_step,
    .dump       = txa16_1_dump,
};
