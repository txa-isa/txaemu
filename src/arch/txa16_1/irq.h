// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>

#pragma once

#include "emu/cpu.h"
#include "state.h"

// Push PC/SR, switch to IRQ bank, load handler from the given IVT address.
void interrupt_entry(txa_cpu_t *cpu, txa16_1_state_t *s, uint16_t vec_addr);

// Check pending NMI / maskable IRQs and call interrupt_entry if needed.
void check_interrupts(txa_cpu_t *cpu, txa16_1_state_t *s);
