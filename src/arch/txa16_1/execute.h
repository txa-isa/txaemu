// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>
//
// Execute sub-function declarations — one per opcode group.
// txa16_1.c dispatches to these from execute().

#pragma once

#include <stdint.h>

#include "emu/cpu.h"
#include "state.h"

// execute/alu.c  — opcodes 0x0 (ALU 3-op) and 0x1 (ALU2)
int exec_alu_group (txa_cpu_t *cpu, txa16_1_state_t *s, uint16_t instr);

// execute/imm.c  — opcodes 0x2-0x6 (ADDI, SUBI, ANDI, ORI, XORI)
int exec_imm_group (txa_cpu_t *cpu, txa16_1_state_t *s, uint16_t instr);

// execute/shift.c — opcode 0x9 (SHL, SHR, SAR, ROL)
int exec_shift_group(txa_cpu_t *cpu, txa16_1_state_t *s, uint16_t instr);

// execute/mem.c  — opcodes 0x7 (LDW), 0x8 (STW), 0xA (MOVL/MOVH)
int exec_mem_group (txa_cpu_t *cpu, txa16_1_state_t *s, uint16_t instr);

// execute/flow.c — opcodes 0xB (Bcc), 0xC (JMP/CALL), 0xD (SYS)
int exec_flow_group(txa_cpu_t *cpu, txa16_1_state_t *s, uint16_t instr);

// execute/ctl.c  — opcodes 0xE (CTL), 0xF (COND)
int exec_ctl_group (txa_cpu_t *cpu, txa16_1_state_t *s, uint16_t instr);
