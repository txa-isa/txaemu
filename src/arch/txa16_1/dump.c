// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>

#include <stdio.h>

#include "emu/cpu.h"
#include "state.h"

void txa16_1_dump(txa_cpu_t *cpu)
{
    txa16_1_state_t *s    = cpu->state;
    int              bank = BANK(s);

    printf("PC=%04X  SP=%04X  SR=%04X  [%c%c%c%c %s %s bank%d]\n",
        s->pc, s->sp, s->sr,
        GET_FLAG(s, SR_OV) ? 'V' : '-',
        GET_FLAG(s, SR_N)  ? 'N' : '-',
        GET_FLAG(s, SR_C)  ? 'C' : '-',
        GET_FLAG(s, SR_Z)  ? 'Z' : '-',
        GET_FLAG(s, SR_SAT) ? "SAT" : "   ",
        GET_FLAG(s, SR_IE)  ? "IE"  : "  ",
        bank);

    for (int i = 0; i < 8; i++)
        printf("  R%d=%04X", i, s->gpr[bank][i]);
    putchar('\n');
}
