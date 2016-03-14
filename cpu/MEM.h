#ifndef MEM_H
#define MEM_H

#include "printf.h"
#include "malloc.h"
//#include "cpu_instrs.h"
#include "bgblogo.h"
#include "DATA_TYPES.h"

/* 
 *         Layout of memory (taken from Pan Docs)
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

#define SIZE_KB   8192
#define ROM_SIZE  65536 // You won't be able to compile until after the H file for the rom is copied!
#define VRAM_SIZE 8192
#define MEM_SIZE  0x10000 // 0x0000 - 0xFFFF
#define OAM_SIZE  160 // 0xFE00 - 0xFEA0
#define IO_SIZE 0x80 //0xFF00 - 0XFF7F
#define HRAM_SIZE 127 // 0xFF80 - 0xFFFE

// Additional Memory Addresses 
#define CART_TYPE_ADDR  0x0147
#define RAM_SIZE_ADDR   0x0149



// Memory map
//extern gb_short GB_ROM[ROM_SIZE]; // To be read from a given .h file.
gb_short *_gb_ram;
gb_short vram[VRAM_SIZE];
gb_short _gb_mem[MEM_SIZE];
gb_short _gb_hram[MEM_SIZE];
gb_short _gb_oam[OAM_SIZE];
gb_short _gb_io[IO_SIZE];

// Flags and stuff
gb_short gb_halt;

// Other definitions
gb_short   read8(gb_long addr);
gb_short_s read8s(gb_long addr);
gb_long    read16(gb_long addr);

// Available functions
void write8(gb_long addr, gb_short val);
void write16(gb_long addr, gb_long val);

void setIME(gb_short flag);
gb_short getIME();


#endif
