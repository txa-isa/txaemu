// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>
//
// Instruction field extraction — pure bit-manipulation, no side effects.

#pragma once

#include <stdint.h>

static inline uint8_t  dec_opcode(uint16_t i) { return (uint8_t)((i >> 12) & 0xF); }
static inline uint8_t  dec_rd    (uint16_t i) { return (uint8_t)((i >>  9) & 0x7); }
static inline uint8_t  dec_rs1   (uint16_t i) { return (uint8_t)((i >>  6) & 0x7); }
static inline uint8_t  dec_rs2   (uint16_t i) { return (uint8_t)((i >>  3) & 0x7); }
static inline uint8_t  dec_func  (uint16_t i) { return (uint8_t)( i        & 0x7); }
static inline uint8_t  dec_imm6  (uint16_t i) { return (uint8_t)( i        & 0x3F); }
static inline uint8_t  dec_imm8  (uint16_t i) { return (uint8_t)( i        & 0xFF); }
static inline uint8_t  dec_h     (uint16_t i) { return (uint8_t)((i >>  8) & 0x1); }
static inline uint8_t  dec_cc    (uint16_t i) { return (uint8_t)((i >>  9) & 0x7); }
static inline uint16_t dec_off9  (uint16_t i) { return (uint16_t)(i        & 0x1FF); }
static inline uint8_t  dec_t     (uint16_t i) { return (uint8_t)((i >> 11) & 0x1); }
static inline uint16_t dec_off11 (uint16_t i) { return (uint16_t)(i        & 0x7FF); }
static inline uint8_t  dec_cnt   (uint16_t i) { return (uint8_t)((i >>  7) & 0x3); }
static inline uint8_t  dec_shift_type  (uint16_t i) { return (uint8_t)((i >> 4) & 0x3); }
static inline uint8_t  dec_shift_amount(uint16_t i) { return (uint8_t)( i       & 0xF); }

// Sign-extend a value of width bits to a full 32-bit signed integer.
static inline int32_t sign_ext(uint32_t val, int bits)
{
    uint32_t sign = 1u << (bits - 1);
    return (int32_t)((val ^ sign) - sign);
}
