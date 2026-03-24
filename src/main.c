// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>
//
// txaemu — TXA ISA emulator entry point.

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "emu/cpu.h"
#include "arch/txa16_1/txa16_1.h"

// Registry of known architectures. Add new entries here as new revisions land.
static const txa_arch_t *arch_registry[] = {
    &txa16_1_arch,
    NULL,
};

static const txa_arch_t *lookup_arch(const char *name)
{
    for (int i = 0; arch_registry[i]; i++) {
        if (strcmp(arch_registry[i]->name, name) == 0)
            return arch_registry[i];
    }
    return NULL;
}

static void list_archs(void)
{
    fprintf(stderr, "Available architectures:\n");
    for (int i = 0; arch_registry[i]; i++)
        fprintf(stderr, "  %s (%u-bit, %u KB)\n",
            arch_registry[i]->name,
            arch_registry[i]->word_bits,
            arch_registry[i]->mem_size / 1024);
}

static void usage(const char *argv0)
{
    fprintf(stderr,
        "Usage: %s [options] <binary>\n"
        "\n"
        "Options:\n"
        "  -a <arch>      Architecture to emulate (default: txa16-1)\n"
        "  -l <addr>      Load address in hex (default: 0x0000)\n"
        "  -s <steps>     Maximum steps before stopping (0 = unlimited)\n"
        "  -d             Dump registers after each instruction\n"
        "  -D             Dump registers at exit only\n"
        "  --list-archs   List available architectures and exit\n"
        "  -h             Show this help\n"
        "\n"
        "The binary is loaded at <addr>. The IVT reset vector at 0x0000 must point\n"
        "to the entry point (set by the binary or via -l to the entry directly).\n",
        argv0);
}

static int load_binary(txa_cpu_t *cpu, const char *path, uint32_t load_addr)
{
    FILE *f = fopen(path, "rb");
    if (!f) {
        perror(path);
        return -1;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    if (size < 0) {
        fclose(f);
        perror("ftell");
        return -1;
    }

    uint32_t avail = cpu->mem_size - load_addr;
    if ((uint32_t)size > avail) {
        fprintf(stderr, "txaemu: binary too large (%ld bytes) for address space "
                        "(%" PRIu32 " bytes available at 0x%04X)\n",
                size, avail, load_addr);
        fclose(f);
        return -1;
    }

    size_t n = fread(cpu->mem + load_addr, 1, (size_t)size, f);
    fclose(f);

    if ((long)n != size) {
        fprintf(stderr, "txaemu: short read (%zu of %ld bytes)\n", n, size);
        return -1;
    }

    fprintf(stderr, "txaemu: loaded %ld bytes at 0x%04X\n", size, load_addr);
    return 0;
}

int main(int argc, char *argv[])
{
    const char *arch_name  = "txa16-1";
    const char *binary     = NULL;
    uint32_t    load_addr  = 0x0000;
    uint64_t    max_steps  = 0;
    bool        dump_each  = false;
    bool        dump_exit  = false;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            usage(argv[0]);
            return 0;
        }
        if (strcmp(argv[i], "--list-archs") == 0) {
            list_archs();
            return 0;
        }
        if (strcmp(argv[i], "-a") == 0 && i + 1 < argc) {
            arch_name = argv[++i];
        } else if (strcmp(argv[i], "-l") == 0 && i + 1 < argc) {
            load_addr = (uint32_t)strtoul(argv[++i], NULL, 16);
        } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            max_steps = (uint64_t)strtoull(argv[++i], NULL, 10);
        } else if (strcmp(argv[i], "-d") == 0) {
            dump_each = true;
        } else if (strcmp(argv[i], "-D") == 0) {
            dump_exit = true;
        } else if (argv[i][0] != '-') {
            binary = argv[i];
        } else {
            fprintf(stderr, "txaemu: unknown option: %s\n", argv[i]);
            usage(argv[0]);
            return 1;
        }
    }

    if (!binary) {
        fprintf(stderr, "txaemu: no binary specified\n");
        usage(argv[0]);
        return 1;
    }

    const txa_arch_t *arch = lookup_arch(arch_name);
    if (!arch) {
        fprintf(stderr, "txaemu: unknown architecture '%s'\n", arch_name);
        list_archs();
        return 1;
    }

    txa_cpu_t *cpu = txa_cpu_create(arch);
    if (!cpu) {
        fprintf(stderr, "txaemu: out of memory\n");
        return 1;
    }

    txa_cpu_reset(cpu);

    if (load_binary(cpu, binary, load_addr) < 0) {
        txa_cpu_destroy(cpu);
        return 1;
    }

    // Re-apply reset so the reset vector (which the binary may have set) takes effect.
    arch->reset(cpu);

    fprintf(stderr, "txaemu: running %s, PC=0x%04X\n",
            arch->name, ((txa16_1_state_t *)cpu->state)->pc);

    if (dump_each) {
        uint64_t step = 0;
        while (!cpu->halted) {
            arch->dump(cpu);
            int ret = txa_cpu_step(cpu);
            if (ret < 0) {
                fprintf(stderr, "txaemu: illegal instruction at step %" PRIu64 "\n", step);
                txa_cpu_destroy(cpu);
                return 1;
            }
            if (max_steps && ++step >= max_steps)
                break;
        }
    } else {
        int ret = txa_cpu_run(cpu, max_steps);
        if (ret < 0) {
            txa_cpu_destroy(cpu);
            return 1;
        }
    }

    if (dump_exit || dump_each)
        arch->dump(cpu);

    txa_cpu_destroy(cpu);
    return 0;
}
