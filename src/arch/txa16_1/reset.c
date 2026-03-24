// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>

#include <string.h>

#include "emu/cpu.h"
#include "state.h"

void txa16_1_reset(txa_cpu_t *cpu)
{
    txa16_1_state_t *s = cpu->state;
    memset(s, 0, sizeof(*s));
    s->sp = TXA16_1_SP_RESET;
    s->sr = 0x0000; // IE=0, BANK=0, SAT=0, all flags clear
    s->pc = mem_rw(cpu, IVT_RESET);
}
