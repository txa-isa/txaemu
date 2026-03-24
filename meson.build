project('txaemu', 'c',
  version : '0.1.0',
  license : 'GPL-3.0-or-later',
  default_options : [
    'c_std=c11',
    'warning_level=2',
  ])

inc_emu = include_directories('src')

emu_sources = files(
  'src/main.c',
  'src/emu/cpu.c',
  'src/arch/txa16_1/alu.c',
  'src/arch/txa16_1/irq.c',
  'src/arch/txa16_1/reset.c',
  'src/arch/txa16_1/step.c',
  'src/arch/txa16_1/dump.c',
  'src/arch/txa16_1/txa16_1.c',
  'src/arch/txa16_1/execute/alu.c',
  'src/arch/txa16_1/execute/imm.c',
  'src/arch/txa16_1/execute/shift.c',
  'src/arch/txa16_1/execute/mem.c',
  'src/arch/txa16_1/execute/flow.c',
  'src/arch/txa16_1/execute/ctl.c',
)

executable('txaemu',
  sources             : emu_sources,
  include_directories : inc_emu,
  install             : true)
