#include "printf.h"

struct {
    gb_short A;
    gb_short F;
    gb_short B;
    gb_short C;
    gb_short D;
    gb_short E;
    gb_short H;
    gb_short L;

    gb_long SP;
    gb_long PC;

} _cpr;


void init_cpu() {
    _cpr.AF = 0;
    _cpr.BC = 0;
    _cpr.DE = 0;
    _cpr.HL = 0;
    _cpr.PC = 0x0100;
    _cpr.SP = 0xFFFE;
}

// Mem access
gb_short cpu_read8() {
    return _gb_rom[_cpr.PC++]; 
}

gb_short cpu_read16() {
    gb_short t = _gb_rom[_cpr.PC];
    _cpr.PC += 2;
    return t;
}

// long register read/write
void write16BC(gb_long l) {
    _cpr.B = (gb_short)(l>>8);
    _cpr.C = (gb_short)(l&0xFF);
}
void write16DE(gb_long l) {
    _cpr.D = (gb_short)(l>>8);
    _cpr.E = (gb_short)(l&0xFF);
}
void write16HL(gb_long l) {
    _cpr.D = (gb_short)(l>>8);
    _cpr.E = (gb_short)(l&0xFF);
}
void write16SP(gb_long l) {
    _cpr.D = (gb_short)(l>>8);
    _cpr.E = (gb_short)(l&0xFF);
}

void exec_op(gb_short op_code) {
    case 0x00:
        break;
    case 0x01:
        _cpr.
        
}
