#include "MEM.h"

gb_short read8(gb_long addr) {
    return _gb_rom[addr];
}
gb_short_s read8s(gb_long addr) {
    return *(gb_short_s*)(&_gb_rom[addr]);
}

gb_long read16(gb_long addr) {
    return *(gb_long*)(&_gb_rom[addr]);
}

void write8(gb_long addr, gb_short val) { _gb_rom[addr] = val; }
void write16(gb_long addr, gb_long val) { *(_gb_long)(&_gb_rom[addr]) = val; }
