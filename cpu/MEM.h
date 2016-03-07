#ifndef MEM_H
#define MEM_H

#include <stdint.h>

/* 
 *         Layout of memory (from Pan Docs)
 * ----------------------------------------------
 * 0000-3FFF   16KB ROM Bank 00     (in cartridge, fixed at bank 00)
 * 4000-7FFF   16KB ROM Bank 01..NN (in cartridge, switchable bank number)
 * 8000-9FFF   8KB Video RAM (VRAM) (switchable bank 0-1 in CGB Mode)
 * A000-BFFF   8KB External RAM     (in cartridge, switchable bank, if any)
 * C000-CFFF   4KB Work RAM Bank 0 (WRAM)
 * D000-DFFF   4KB Work RAM Bank 1 (WRAM)  (switchable bank 1-7 in CGB Mode)
 * E000-FDFF   Same as C000-DDFF (ECHO)    (typically not used)
 * FE00-FE9F   Sprite Attribute Table (OAM)
 * FEA0-FEFF   Not Usable
 * FF00-FF7F   I/O Ports
 * FF80-FFFE   High RAM (HRAM)
 * FFFF        Interrupt Enable Register
 */

#define SIZE_KB  8192

// Units of work
typedef uint16_t gb_long;
typedef int16_t  gb_long_s;
typedef uint8_t  gb_short;
typedef int8_t   gb_short_s;

// Memory map.
static gb_short _gb_rom[ROM_SIZE*SIZE_KB]; 

gb_short read8(gb_long addr);
gb_long read16(gb_long addr);

void write8(gb_long addr, gb_short val);
void write16(gb_long addr, gb_long val);


#endif
