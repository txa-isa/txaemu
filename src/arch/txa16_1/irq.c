// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>

#include "irq.h"
#include "emu/cpu.h"

void interrupt_entry(txa_cpu_t *cpu, txa16_1_state_t *s, uint16_t vec_addr)
{
    // Push next-PC then SR (PC was pre-advanced in step.c before execute).
    s->sp -= 2; mem_ww(cpu, s->sp, s->pc);
    s->sp -= 2; mem_ww(cpu, s->sp, s->sr);

    s->sr |=  (uint16_t)SR_BANK; // switch to IRQ register bank
    s->sr &= (uint16_t)~(uint16_t)SR_IE;  // disable further maskable IRQs

    s->pc = mem_rw(cpu, vec_addr);
    cpu->halted = false;
}

void check_interrupts(txa_cpu_t *cpu, txa16_1_state_t *s)
{
    if (cpu->pending_nmi) {
        cpu->pending_nmi = false;
        interrupt_entry(cpu, s, IVT_NMI);
        return;
    }
    if (GET_FLAG(s, SR_IE) && cpu->pending_irq) {
        for (int i = 0; i < 8; i++) {
            if (cpu->pending_irq & (1u << i)) {
                cpu->pending_irq &= ~(1u << i);
                interrupt_entry(cpu, s, (uint16_t)(IVT_IRQ0 + i * 2));
                return;
            }
        }
    }
}
