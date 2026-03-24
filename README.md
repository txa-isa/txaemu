# txaemu

Emulator for the TXA ISA family (The XISC Architecture).

**License:** GNU General Public License v3.0 or later

## Building

Requires a C11 compiler and [Meson](https://mesonbuild.com/) ≥ 0.57.

```sh
meson setup build && ninja -C build
```

Install system-wide:

```sh
ninja -C build install
```

### Assembler

The assembler (`asm-txa`) lives in the `asm/` submodule. Build it separately:

```sh
cd asm && meson setup build && ninja -C build
```

The binary is at `asm/build/asm-txa`. See `asm/README.md` for usage.

## Usage

```
txaemu [options] <binary>

Options:
  -a <arch>      Architecture to emulate (default: txa16-1)
  -l <addr>      Load address in hex (default: 0x0000)
  -s <steps>     Maximum steps before stopping (0 = unlimited)
  -d             Dump registers after each instruction
  -D             Dump registers at exit only
  --list-archs   List available architectures and exit
  -h             Show help
```

### Running a binary

The emulator loads a raw binary into memory at the given address, then
performs a reset. The reset vector at `0x0000` must point to the entry
point; it is typically set by the binary itself (first two bytes).

```sh
# Load binary at address 0x0000 and run
txaemu program.bin

# Load at a custom address
txaemu -l 0x0100 program.bin

# Run with register dump after each instruction
txaemu -d program.bin

# Limit to 1000 instructions
txaemu -s 1000 program.bin
```

### Register dump format

```
PC=0022  SP=FFFE  SR=0003  [--CZ         bank0]
  R0=000A  R1=000A  R2=0000  R3=0000  R4=0000  R5=0000  R6=0000  R7=0000
```

Flag characters in order: V (overflow), N (negative), C (carry), Z (zero).
A dash means the flag is clear. SAT and IE show as `SAT`/`IE` when set, spaces when clear.

## Architecture support

Each ISA variant is a self-contained module under `src/arch/`. To add a new
architecture:

1. Create `src/arch/<name>/` with a header and a `.c` file.
2. Define and export a `const txa_arch_t` descriptor (see `txa16_1_arch`
   in `src/arch/txa16_1/txa16_1.h` as the reference).
3. Register the pointer in the `arch_registry[]` array in `src/main.c`.
4. Add the new `.c` file to `emu_sources` in `meson.build`.

The generic CPU core (`src/emu/cpu.h`, `src/emu/cpu.c`) is architecture-
independent. It provides memory allocation, little-endian word helpers, the
run loop, and the IRQ/NMI raise API.

## TXA16-1 summary

| Parameter         | Value                        |
|-------------------|------------------------------|
| Word size         | 16 bits                      |
| Instruction width | 16 bits (fixed)              |
| Address space     | 64 KB (byte-addressable)     |
| GPR               | 8 × 16-bit (R0–R7, banked)   |
| Special registers | PC, SP, SR                   |
| Byte order        | Little-endian                |
| Stack             | Grows downward, SP = 0xFFFE  |
| IVT               | 0x0000–0x0013 (10 vectors)   |

Full specification: `spec/txa16-1/docs/`.

## Tests

```sh
sh tests/run_tests.sh
```

Assembles each test in `tests/` with `asm-txa`, runs it, and checks `R0` against the expected value.
