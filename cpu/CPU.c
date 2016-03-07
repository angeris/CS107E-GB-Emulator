#include "printf.h"
#include "CPU.h"

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
    // TODO: get the correct init codes.
}

// Mem access
gb_short cpu_read8() {
    return _gb_rom[_cpr.PC++]; 
}


// TODO: Fix this. Should read gb_long
gb_short cpu_read16() {
    gb_short t = _gb_rom[_cpr.PC];
    _cpr.PC += 2;
    return t;
}

// TODO: Fix this... not rom.
void ram_write8(gb_long addr, gb_short s) {
    _gb_rom[addr] = s;
}

void ram_write16(gb_long addr, gb_long s) {
    *(gb_long*)(&_gb_rom[addr]) = s;
}

gb_short read8(gb_long addr) {
    return _gb_rom[addr];
}

gb_long read16(gb_long addr) {
    return (gb_long)(_gb_rom[addr]) << 8 | (gb_long)(_gb_rom[addr+1]);
}

// long register read/write
static inline void setBC(gb_long l) {
    _cpr.B = (gb_short)(l>>8);
    _cpr.C = (gb_short)(l&0xFF);
}

static inline void setDE(gb_long l) {
    _cpr.D = (gb_short)(l>>8);
    _cpr.E = (gb_short)(l&0xFF);
}

static inline void setHL(gb_long l) {
    _cpr.H = (gb_short)(l>>8);
    _cpr.L = (gb_short)(l&0xFF);
}

// Read helpers (for clarity)
// TODO: Cast to gb_longs before shifts.
static inline gb_short A() { return _cpr.A; }
static inline gb_short F() { return _cpr.F; }
static inline gb_short B() { return _cpr.B; }
static inline gb_short C() { return _cpr.C; }
static inline gb_short D() { return _cpr.D; }
static inline gb_short E() { return _cpr.E; }
static inline gb_short H() { return _cpr.H; }
static inline gb_short L() { return _cpr.L; }
static inline gb_long SP() { return _cpr.SP; }
static inline gb_long BC() { return _cpr.B << 8 | _cpr.C; }
static inline gb_long DE() { return _cpr.D << 8 | _cpr.E; }
static inline gb_long HL() { return _cpr.H << 8 | _cpr.L; }

static inline void setA(gb_short a) { _cpr.A = a; }
static inline void setF(gb_short a) { _cpr.F = a; }
static inline void setB(gb_short a) { _cpr.B = a; }
static inline void setC(gb_short a) { _cpr.C = a; }
static inline void setD(gb_short a) { _cpr.D = a; }
static inline void setE(gb_short a) { _cpr.E = a; }
static inline void setH(gb_short a) { _cpr.H = a; }
static inline void setL(gb_short a) { _cpr.L = a; }
static inline void setSP(gb_long a) { _cpr.SP = a; }

// Flags
void setZero(gb_short i) {
    i = !!i;
    _cpr.F &= ~F_ZERO;
    _cpr.F |= i<<7;
}
void setSubs(gb_short i) {
    i = !!i;
    _cpr.F &= ~F_SUBS;
    _cpr.F |= i<<6;
}
void setHalf(gb_short i) {
    i = !!i;
    _cpr.F &= ~F_HALF;
    _cpr.F |= i<<5;
}
void setCarr(gb_short i) {
    i = !!i;
    _cpr.F &= ~F_CARR;
    _cpr.F |= i<<4;
}

gb_short isZero() { return !!(F_ZERO & _cpr.F); }
gb_short isSubs() { return !!(F_SUBS & _cpr.F); }
gb_short isHalf() { return !!(F_HALF & _cpr.F); }
gb_short isCarr() { return !!(F_CARR & _cpr.F); }


// Helpers
static gb_short inc8(gb_short reg) {
    reg++;
    setzero(!reg);
    issubs(0);
    ishalf(!(reg & 0xf));
    return reg;
}
static gb_long inc16(gb_long reg) {
    return reg+1;
}
static gb_short dec8(gb_short reg) {
    reg--;
    setzero(!reg);
    issubs(1);
    ishalf(!(reg & 0xf));
    return reg;
}
static gb_long dec16(gb_long reg) {
    return reg-1;
}

static gb_short rot8_l(gb_short reg) {
    gb_short lbit = !!(reg & 1<<7);
    reg = reg<<1 | lbit; 
    setcarr(lbit);
    setzero(0);
    sethalf(0);
    setsubs(0);
    return reg;
}

static gb_short rot8_r(gb_short reg) {
    gb_short rbit = reg & 1;
    reg = (reg >> 1) | (rbit << 7);
    setcarr(rbit);
    setzero(0);
    sethalf(0);
    setsubs(0);
    return reg;
}

static gb_short rot8_r_carry(gb_short reg) {
    gb_short rbit = reg & 1;
    reg = (reg >> 1) | (isCarr() << 7); 
    setCarr(rbit);
    setZero(0);
    setHalf(0);
    setSubs(0);
    return reg;
}

static gb_short rot8_l_carry(gb_short reg) {
    gb_short lbit = !!(reg & (1 << 7));
    reg = (reg >> 1) | (isCarr() << 7); 
    setCarr(lbit);
    setZero(0);
    setHalf(0);
    setSubs(0);
    return reg;
}

static gb_long add16(gb_long a, gb_long b) {
    unsigned f = (unsigned)a + (unsigned)b;
    setSubs(0);
    setCarr(f>0xFFFF);
    setHalf((a&0xFFF) + (b&0xFFF) > 0xFFF);
    return (gb_long)f;
}

// CPU operations
void exec_op(gb_short op_code) {
    switch(op_code) {
    case 0x00:
      break;
    case 0x01:
      setBC(cpu_read16()); 
      break;
    case 0x02:
      ram_write8(BC(), A());
      break;
    case 0x03:
      setBC( inc16( BC() ) );
      break;
    case 0x04:
      setB( inc8( B() ) );
      break;
    case 0x05:
      setB( dec8( B() ) );
      break;
    case 0x06:
      setB(cpu_read8());
      break;
    case 0x07:
      setA( rot8_l( A() ) );
      break;
    case 0x08:
      ram_write16(cpu_read16(), SP());
      break;
    case 0x09:
      setHL( add16(HL(), BC()) );
      break; 
    case 0x0A:
      setA( read16(BC()) );
      break;
    case 0x0B:
      setBC( dec16( BC() ) );
      break;
    case 0x0C:
      setC( inc8( C() ) );
      break;
    case 0x0D:
      setC( dec8( C() ) );
      break;
    case 0x0E:
      setC( cpu_read8() );
      break;
    case 0x0F:
      setA( rot8_r( A() ) );
      break;
    case 0x10:
      break;
    case 0x11:
      setDE(cpu_read16()); 
      break;
    case 0x12:
      ram_write8((DE), A());
      break;
    case 0x13:
      setDE( inc16( DE() ) );
    case 0x14:
      setD( inc8( D() ) );
      break;
    case 0x15:
      setD( dec8( D() ) );
      break;
    case 0x16:
      setD(cpu_read8());
      break;
    case 0x17:
      setA( rot8_l_carry( A() ) );
      break;
    case 0x18:
      setPC( _cpr.PC + read8s(_cpr.PC) + 1);
      break;
    case 0x19:
      setHL( add16(HL(), DE()) );
      break;
    case 0x1A:
      setA( read16(DE()) );
      break;
    case 0x1B:
      setDE( dec16( DE() ) );
      break;
    case 0x1C:
      setE( inc8( E() ) );
      break;
    case 0x1D:
      setE( dec8( E() ) );
      break;
    case 0x1E:
      setE( cpu_read8());
      break;
    case 0x1F:
      setA(rot8_r_carry(A()));
      break;
      

	     
    }

}
