#include "MEM.h"

static unsigned _mbc;
static unsigned _rom_bank;
static unsigned _ram_bank;
static unsigned _ram_enabled;

void init() {
    // Allocate _gb_ram according to cartridge specs 
}

gb_short read8(gb_long addr) {
    if(addr < 0x4000) 
        return GB_ROM[addr];
    if(addr >= 0x4000  && addr < 0x8000) {
        if(_mbc == 0)
            return GB_ROM[addr];
        else
            return GB_ROM[addr - 0x4000 + _rom_bank << 14]; // multiply by 0x4000
    }
    if(addr >= 0x8000 && addr < 0xA000)
        return _vram[addr - 0x8000];

    if(addr >= 0xA000 && addr < 0xC000) 
        if(_mbc != 0) 
            return _gb_ram[addr - 0xA000 + _ram_bank << 13] // multiply by 0x2000

                return _gb_mem[addr];
}

gb_short_s read8s(gb_long addr) {
    return *(gb_short_s*)(&read8(addr));
}

gb_long read16(gb_long addr) {
    return (gb_long)read8(addr) << 8 | (gb_long)read8(addr+1);
}

void write8(gb_long addr, gb_short val) {
    // TODO: Implement Joypad/specific address stuff
    if(addr < 0x8000 && _mbc) {
        if(addr < 0x2000) 
            _ram_enabled = (0xF & val == 0xA);

        else if(addr >= 0x2000 && addr < 0x4000)
            _rom_bank = val;
        else if(addr >= 0x4000 && addr < 0x6000)
            _ram_bank = val;
    }
    else if(addr >= 0x8000 && addr < 0xA000)
        _vram[addr - 0x8000] = val;
    else if(addr >= 0xA000 && addr < 0xC000 && _ram_enabled && _mbc)
        _gb_ram[addr - 0xA000 + _ram_bank << 13] = val;
    else if(addr >= 0xC000 && addr < 0xD000) {
        _gb_mem[addr] = val;
        _gb_mem[addr + 0x2000] = val; // Echo, as PanDocs
    }
    else if(addr >= 0xD000 && addr < 0xE000) {
        _gb_mem[addr] = val;
        _gb_mem[addr - 0x2000] = val; // Echo, as PanDocs
    }
    else
        _gb_mem[addr] = val;
}

void write16(gb_long addr, gb_long val) { *(_gb_long)(&GB_ROM[addr]) = val; }
