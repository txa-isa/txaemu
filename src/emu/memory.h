// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>
//
// Raw little-endian memory helpers operating on a flat byte buffer.
// No dependency on txa_cpu_t — can be used independently.
// cpu.h wraps these with txa_cpu_t* convenience functions.

#pragma once

#include <stdint.h>

static inline uint8_t mem_buf_rb(const uint8_t *buf, uint32_t size, uint32_t addr)
{
    return buf[addr & (size - 1)];
}

static inline void mem_buf_wb(uint8_t *buf, uint32_t size, uint32_t addr, uint8_t val)
{
    buf[addr & (size - 1)] = val;
}

static inline uint16_t mem_buf_rw(const uint8_t *buf, uint32_t size, uint32_t addr)
{
    uint32_t mask = size - 1;
    return (uint16_t)buf[addr & mask]
         | ((uint16_t)buf[(addr + 1) & mask] << 8);
}

static inline void mem_buf_ww(uint8_t *buf, uint32_t size, uint32_t addr, uint16_t val)
{
    uint32_t mask = size - 1;
    buf[ addr      & mask] = (uint8_t)(val & 0xFF);
    buf[(addr + 1) & mask] = (uint8_t)(val >> 8);
}
