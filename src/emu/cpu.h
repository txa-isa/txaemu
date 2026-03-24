// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>
//
// Generic CPU abstraction.  Each architecture implements txa_arch_t and
// registers its descriptor via the arch_registry in main.c.

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "memory.h"

struct txa_cpu;

typedef struct txa_arch {
    const char *name;
    uint32_t    word_bits;
    uint32_t    mem_size;   // must be a power of two
    size_t      state_size;

    void (*reset)(struct txa_cpu *cpu);
    int  (*step) (struct txa_cpu *cpu); // 0=ok, -1=fatal
    void (*dump) (struct txa_cpu *cpu);
} txa_arch_t;

typedef struct txa_cpu {
    const txa_arch_t *arch;
    uint8_t          *mem;
    uint32_t          mem_size;
    void             *state;
    bool              halted;
    uint32_t          pending_irq;  // bitmask, bit i = IRQi pending
    bool              pending_nmi;
} txa_cpu_t;

txa_cpu_t *txa_cpu_create (const txa_arch_t *arch);
void       txa_cpu_destroy(txa_cpu_t *cpu);
void       txa_cpu_reset  (txa_cpu_t *cpu);
int        txa_cpu_step   (txa_cpu_t *cpu);
int        txa_cpu_run    (txa_cpu_t *cpu, uint64_t max_steps);
void       txa_cpu_raise_irq(txa_cpu_t *cpu, int irq);
void       txa_cpu_raise_nmi(txa_cpu_t *cpu);

// txa_cpu_t-level wrappers around the raw buffer helpers in memory.h

static inline uint8_t mem_rb(txa_cpu_t *cpu, uint32_t addr)
{
    return mem_buf_rb(cpu->mem, cpu->mem_size, addr);
}

static inline void mem_wb(txa_cpu_t *cpu, uint32_t addr, uint8_t val)
{
    mem_buf_wb(cpu->mem, cpu->mem_size, addr, val);
}

static inline uint16_t mem_rw(txa_cpu_t *cpu, uint32_t addr)
{
    return mem_buf_rw(cpu->mem, cpu->mem_size, addr);
}

static inline void mem_ww(txa_cpu_t *cpu, uint32_t addr, uint16_t val)
{
    mem_buf_ww(cpu->mem, cpu->mem_size, addr, val);
}
