#include "MEM.h"
#include "GPU.h"

static unsigned _mbc;
static unsigned _rom_bank;
static unsigned _ram_bank;
static gb_short _ram_enabled;
static gb_short _ime_enabled;
static gb_short _interrupts;

void gpu_testing() {
    printf("GPU TESTING VRAM INT-----------\n");
    for(int i = 0x8000; i < 0x9FFF; i+=2) {
        write8(i, 0xFF);
        write8(i+1, 0xD8); // binary for 11011000
    }

    write8(SCROLLY, 0);
    write8(SCROLLX, 0);
    write8(WINDOWY, 0);
    write8(WINDOWX, 0);
    write8(LCDY, 0);

    write8(LCD_CONTROL_REG, 0xFF);
    gb_short lcr = read8(LCD_CONTROL_REG);
    printf("LCD CONTROL REG = %x\n",lcr);

    write8(BGPAL, 0xD8); // binary for 11011000
    write8(OBPAL0, 0xD8); // binary for 11011000
    write8(OBPAL1, 0xD8); // binary for 11011000
    write8(SPRITE_OAM, 0x00);


}

extern gb_short get_joypad_input(); //XXX: Or whatever this is

void init() {
    switch(GB_ROM[RAM_SIZE_ADDR]) {
    case 0x00:
        _gb_ram = (gb_short*)malloc(0);
        break;
    case 0x01:
        _gb_ram = (gb_short*)malloc(2048);
        break;
    case 0x02:
        _gb_ram = (gb_short*)malloc(8192);
        break;
    case 0x03:
        _gb_ram = (gb_short*)malloc(32*1024);
        break;
    default:
        printf("Error reading RAM_SIZE_ADDR with value : %02x", GB_ROM[RAM_SIZE_ADDR]);
        break;
    }
}

gb_short read8(gb_long addr) {
    if(addr < 0x4000) 
        return GB_ROM[addr];
    if(addr >= 0x4000  && addr < 0x8000) {
        if(_mbc == 0)
            return GB_ROM[addr];
        else
            return GB_ROM[addr - 0x4000 + (_rom_bank << 14)]; // multiply by 0x4000
    }
    if(addr >= 0x8000 && addr < 0xA000)
        return vram[addr - 0x8000];

    if(addr >= 0xA000 && addr < 0xC000) {
        if(_mbc != 0 && _ram_enabled) 
            return _gb_ram[addr - 0xA000 + (_ram_bank << 13)]; // multiply by 0x2000
        else 
            return _gb_mem[addr]; // If ram isn't enabled or we don't have additional ram, just write to _gb_mem and return whatever's in it.
    }
    if(addr >= 0xC000 && addr < 0xFE00)
        return _gb_mem[addr]; // Not strictly necessary, but nice to be explicit
    if(addr >= 0xFE00 && addr < 0xFEA0) 
        return _gb_oam[addr - 0xFE00];
    if(addr >= 0xFEA0 && addr < 0xFF00)
        return 0;
    if(addr >= 0xFF00 && addr < 0xFF80)
        return _gb_io[addr - 0xFF00];
    if(addr >= 0xFF80 && addr < 0xFFFF)
        return _gb_hram[addr - 0xFF80];
    if(addr == 0xFFFF)
        return _interrupts;

    printf("Something is accessing wrong memory at : 0x%04x (READ)", addr);
    return _gb_mem[addr];
}

gb_short_s read8s(gb_long addr) {
    gb_short _temp = read8(addr);
    return *(gb_short_s*)(&_temp);
}

gb_long read16(gb_long addr) {
    return (gb_long)read8(addr+1) << 8 | (gb_long)read8(addr);
}

void write8(gb_long addr, gb_short val) {
    if(addr < 0x8000 && _mbc==3) {
        if(addr < 0x2000) 
            _ram_enabled = ((0xF & val) == 0xA);
        else if(addr >= 0x2000 && addr < 0x4000)
            _rom_bank = val;
        else if(addr >= 0x4000 && addr < 0x6000)
            _ram_bank = val;
        // TODO: Implement Latch Clock Data stuff
    }
    else if(addr >= 0x8000 && addr < 0xA000)
        vram[addr - 0x8000] = val;
    else if(addr >= 0xA000 && addr < 0xC000 && _ram_enabled) { 
        if(_mbc && _ram_enabled) _gb_ram[addr - 0xA000 + (_ram_bank << 13)] = val;
        else _gb_mem[addr] = val; // Same as read8
    }
    else if(addr >= 0xC000 && addr < 0xE000) {
        _gb_mem[addr] = val;
        _gb_mem[addr + 0x2000] = val; // Echo, as PanDocs
    }
    else if(addr >= 0xD000 && addr < 0xFE00) {
        _gb_mem[addr] = val;
        _gb_mem[addr - 0x2000] = val; // Echo, as PanDocs
    }
    else if(addr >= 0xFE00 && addr < 0xFEA0) 
        _gb_oam[addr - 0xFE00] = val;
    else if(addr >= 0xFF00 && addr < 0XFF80) 
        _gb_io[addr - 0xFF00] = val;
    else if(addr >= 0xFF80 && addr < 0xFFFF)
        _gb_hram[addr - 0xFF80] = val;
    else if(addr == 0xFFFF)
        _interrupts = val;
    else
        _gb_mem[addr] = val;
}

void write16(gb_long addr, gb_long val) { 
    write8(addr, (gb_short)val);
    write8(addr+1, (gb_short)(val>>8));
}

void setIME(gb_short flag) { _ime_enabled = !!flag; }
gb_short getIME() { return _ime_enabled; }
