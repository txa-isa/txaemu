// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "cpu.h"

txa_cpu_t *txa_cpu_create(const txa_arch_t *arch)
{
    txa_cpu_t *cpu = calloc(1, sizeof(*cpu));
    if (!cpu)
        return NULL;

    cpu->arch     = arch;
    cpu->mem_size = arch->mem_size;
    cpu->mem      = calloc(1, arch->mem_size);
    cpu->state    = calloc(1, arch->state_size);

    if (!cpu->mem || !cpu->state) {
        txa_cpu_destroy(cpu);
        return NULL;
    }

    return cpu;
}

void txa_cpu_destroy(txa_cpu_t *cpu)
{
    if (!cpu)
        return;
    free(cpu->mem);
    free(cpu->state);
    free(cpu);
}

void txa_cpu_reset(txa_cpu_t *cpu)
{
    memset(cpu->mem, 0, cpu->mem_size);
    cpu->halted      = false;
    cpu->pending_irq = 0;
    cpu->pending_nmi = false;
    cpu->arch->reset(cpu);
}

int txa_cpu_step(txa_cpu_t *cpu)
{
    return cpu->arch->step(cpu);
}

int txa_cpu_run(txa_cpu_t *cpu, uint64_t max_steps)
{
    uint64_t n = 0;
    while (!cpu->halted) {
        int ret = cpu->arch->step(cpu);
        if (ret < 0) {
            fprintf(stderr, "txaemu: fatal error at step %" PRIu64 "\n", n);
            return -1;
        }
        if (max_steps && ++n >= max_steps)
            break;
    }
    return 0;
}

void txa_cpu_raise_irq(txa_cpu_t *cpu, int irq)
{
    if (irq >= 0 && irq < 8)
        cpu->pending_irq |= (1u << irq);
}

void txa_cpu_raise_nmi(txa_cpu_t *cpu)
{
    cpu->pending_nmi = true;
}
