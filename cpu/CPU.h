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


#endif
