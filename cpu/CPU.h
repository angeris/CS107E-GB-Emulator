#ifndef CPU_H
#define CPU_H

#include "printf.h"
#include "MEM.h"
#include "DATA_TYPES.h"

void init_cpu();
void cpu_step();
void exec_op(gb_short op_code); 

// Mem access
gb_short cpu_read8();
gb_long cpu_read16();

// Flags:

#define F_ZERO (1<<7)
#define F_SUBS (1<<6)
#define F_HALF (1<<5)
#define F_CARR (1<<4)

// Zero
void setZero(gb_short i);
gb_short isZero();
// Subs
void setSubs(gb_short i);
gb_short isSubs();
// Halfcarry
void setHalf(gb_short i);
gb_short isHalf();
// Carry
void setCarr(gb_short i);
gb_short isCarr();

// Interrupts
#define INT_FLAG        0xFF0F
#define INT_VBLANK      1<<0
#define INT_LCDSTAT     1<<1
#define INT_TIMER       1<<2
#define INT_SERIAL      1<<3
#define INT_JOYPAD      1<<4
#define INT_VBLANK_ADDR 0x40
#define INT_LCDC_ADDR   0x48
#define INT_TIMER_ADDR  0x50
#define INT_SERIAL_ADDR 0x58
#define INT_JOYPAD_ADDR 0x60

#endif
