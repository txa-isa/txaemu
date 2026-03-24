// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>
//
// Fetch-predicate-execute loop for one instruction.

#include "emu/cpu.h"
#include "state.h"
#include "irq.h"

// Defined in txa16_1.c — dispatches to the execute/* group functions.
int execute(txa_cpu_t *cpu, txa16_1_state_t *s, uint16_t instr);

int txa16_1_step(txa_cpu_t *cpu)
{
    txa16_1_state_t *s = cpu->state;

    if (cpu->halted) {
        check_interrupts(cpu, s);
        return 0;
    }

    // Fetch and pre-advance PC so all relative targets use (original_PC + 2).
    uint16_t instr = mem_rw(cpu, s->pc);
    s->pc += 2;

    // COND predication: decide skip/execute, consume one slot.
    bool skip = false;
    if (s->cond_remaining > 0) {
        skip = !s->cond_true;
        s->cond_remaining--;
    }

    int ret = 0;
    if (!skip)
        ret = execute(cpu, s, instr);

    if (ret == 0)
        check_interrupts(cpu, s);

    return ret;
}
