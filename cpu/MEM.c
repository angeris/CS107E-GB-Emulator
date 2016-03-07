#include "MEM.h"

gb_short read8(gb_long addr) {
    return _gb_rom[addr]
}
gb_long read16(gb_long addr) {

}

void write8(gb_long addr, gb_short val);
void write16(gb_long addr, gb_long val);
