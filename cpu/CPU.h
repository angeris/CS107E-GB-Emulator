#ifndef CPU_H
#define CPU_H

#include "printf.h"
#include "MEM.h"

void init_cpu();

// Mem access
gb_short cpu_read8();
gb_short cpu_read16();

// long register read/write
void write16BC(gb_long l);
void write16DE(gb_long l);
void write16HL(gb_long l);
void write16SP(gb_long l);

#endif
