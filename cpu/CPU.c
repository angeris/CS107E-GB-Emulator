#include "printf.h"
#include "CPU.h"
#include "Mem.h"

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

//should be left shifts?

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
static inline void setPC(gb_long a) { _cpr.PC = a; }

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
    setSubs(0);
    setHalf(!(reg & 0xf));
    return reg;
}
static gb_long inc16(gb_long reg) {
    return reg+1;
}
static gb_short dec8(gb_short reg) {
    reg--;
    setzero(!reg);
    setSubs(1);
    setHalf(!(reg & 0xf));
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

static void jump_relative() {
	setPC(_cpr.PC + read8s(_cpr.PC) + 1));
	
}

static void jump_relative_with_cond(int cond) {
	if (cond) {
		jump_relative();
	}
	else {
		_cpr.PC++;
	}	
}

static gb_long add16(gb_long a, gb_long b) {
    unsigned f = (unsigned)a + (unsigned)b;
    setSubs(0);
    setCarr(f>0xFFFF);
    setHalf((a&0xFFF) + (b&0xFFF) > 0xFFF);
    return (gb_long)f;
}

static gb_short add8(gb_short val) {
	gb_short a = A();
	unsigned f = (unsigned)a + (unsigned)val;
	
	setSubs(0);
	setCarr(f > 0xFF);
	setHalf((a&0xF) + (b&0xF) > 0xF);
	setZero(f == 0);
	return (gb_short)f;
}

static gb_short subtract8(gb_short val) {
	gb_short a = A();
	unsigned f = (unsigned)a - (unsigned)val;
	setSubs(1);
	setCarr((gb_short)f > a);
	setHalf((a&0xF) < (val&0xF));
	setZero(f == 0);
	return (gb_short)f;
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
      setA( read8(BC()) );
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
      ram_write8(DE(), A());
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
      jump_relative();
      break;
    case 0x19:
      setHL( add16(HL(), DE()) );
      break;
    case 0x1A:
      setA( read8(DE()) );
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
	  
	  
	case 0x20:
		jump_relative_with_cond(!isZero());
		break;
    case 0x21:
	  setHL(cpu_read16()); 
      break;
    case 0x22:
      ram_write8((HL), A());
	  setHL( inc16( HL() ) );
      break;
    case 0x23:
      setHL( inc16( HL() ) );
    case 0x24:
      setH( inc8( H() ) );
      break;
    case 0x25:
      setH( dec8( H() ) );
      break;
    case 0x26:
      setH(cpu_read8());
      break;
    case 0x27:
      //DAA();
      break;
    case 0x28:
      jump_relative_with_cond(isZero());
      break;
    case 0x29:
      setHL( add16(HL(), HL()) );
      break;
    case 0x2A:
      setA( read8(HL()) );
	  setHL( inc16(HL() ) );
      break;
    case 0x2B:
      setHL( dec16( HL() ) );
      break;
    case 0x2C:
      setL( inc8( L() ) );
      break;
    case 0x2D:
      setL( dec8( L() ) );
      break;
    case 0x2E:
      setL( cpu_read8());
      break;
    case 0x2F:
      //CPL()
      break;
	
	
	case 0x30:
		jump_relative_with_cond(!isCarr());
      break;
    case 0x31:
	  setSP(cpu_read16()); 
      break;
    case 0x32:
      ram_write8(HL(), A());
	  setHL( dec16( HL() ) );
      break;
    case 0x33:
      setSP( inc16( SP() ) );
    case 0x34:
      ram_write16(HL(), inc16( cpu_read16( HL() ) ) );
      break;
    case 0x35:
      ram_write16(HL(), dec16( cpu_read16( HL() ) ) );
      break;
    case 0x36:
      ram_write16(HL(), cpu_read8());
      break;
    case 0x37:
      //SCF();
      breaK;
    case 0x38:
      jump_relative_with_cond(isCarr());
      break;
    case 0x39:
      setHL( add16(HL(), SP()) );
      break;
    case 0x3A:
      setA( read8(HL()) );
	  setHL( dec16(HL() ) );
      break;
    case 0x3B:
      setSP( dec16( SP() ) );
      break;
    case 0x3C:
      setA( inc8( A() ) );
      break;
    case 0x3D:
      setA( dec8( A() ) );
      break;
    case 0x3E:
      setA( cpu_read8());
      break;
    case 0x3F:
      //CCF()
      break;
	

	case 0x40:
		setB( B() );
		break;
    case 0x41:
		setB( C() );
		break;
    case 0x42:
		setB( D() );
		break;
    case 0x43:
		setB( E() );
		break;
	case 0x44:
		setB( H() );
		break;
    case 0x45:
		setB( L() );
		break;
    case 0x46:
		setB( read8( HL() ) );
		break;
    case 0x47:
		setB( A() );
		break;
    case 0x48:
		setC( B() );
		break;
    case 0x49:
		setC( C() );
		break;
    case 0x4A:
		setC( D() );
		break;
    case 0x4B:
		setC( E() );
		break;
    case 0x4C:
		setC( H() );
		break;
	case 0x4D:
		setC( L() );
		break;
    case 0x4E:
		setC( read8( HL() ) );
		break;
    case 0x4F:
		setC( A() );
		break;
	
	case 0x50:
		setD( B() );
		break;
    case 0x51:
		setD( C() );
		break;
    case 0x52:
		setD( D() );
		break;
    case 0x53:
		setD( E() );
		break;
	case 0x54:
		setD( H() );
		break;
    case 0x55:
		setD( L() );
		break;
    case 0x56:
		setD( read8( HL() ) );
		break;
    case 0x57:
		setD( A() );
		break;
    case 0x58:
		setE( B() );
		break;
    case 0x59:
		setE( C() );
		break;
    case 0x5A:
		setE( D() );
		break;
    case 0x5B:
		setE( E() );
		break;
    case 0x5C:
		setE( H() );
		break;
	case 0x5D:
		setE( L() );
		break;
    case 0x5E:
		setE( read8( HL() ) );
		break;
    case 0x5F:
		setE( A() );
		break;
		
		
	case 0x60:
		setH( B() );
		break;
    case 0x61:
		setH( C() );
		break;
    case 0x62:
		setH( D() );
		break;
    case 0x63:
		setH( E() );
		break;
	case 0x64:
		setH( H() );
		break;
    case 0x65:
		setH( L() );
		break;
    case 0x66:
		setH( read8( HL() ) );
		break;
    case 0x67:
		setH( A() );
		break;
    case 0x68:
		setL( B() );
		break;
    case 0x69:
		setL( C() );
		break;
    case 0x6A:
		setL( D() );
		break;
    case 0x6B:
		setL( E() );
		break;
    case 0x6C:
		setL( H() );
		break;
	case 0x6D:
		setL( L() );
		break;
    case 0x6E:
		setL( read8( HL() ) );
		break;
    case 0x6F:
		setL( A() );
		break;
	
	
	case 0x70:
		ram_write8( HL(), B() );
		break;
    case 0x71:
		ram_write8( HL(), C() );
		break;
    case 0x72:
		ram_write8( HL(), D() );
		break;
    case 0x73:
		ram_write8( HL(), E() );
		break;
	case 0x74:
		ram_write8( HL(), H() );
		break;
    case 0x75:
		ram_write8( HL(), L() );
		break;
    case 0x76:
		//halt
		break;
    case 0x77:
		ram_write8( HL(), A() );
		break;
    case 0x78:
		setA( B() );
		break;
    case 0x79:
		setA( C() );
		break;
    case 0x7A:
		setA( D() );
		break;
    case 0x7B:
		setA( E() );
		break;
    case 0x7C:
		setA( H() );
		break;
	case 0x7D:
		setA( L() );
		break;
    case 0x7E:
		setA( read8( HL() ) );
		break;
    case 0x7F:
		setA( A() );
		break;
		
		
	case 0x80:
		setA( A() + B() );
		break;
    case 0x81:
		setA( A() + C() );
		break;
    case 0x82:
		setA( A() + D() );
		break;
    case 0x83:
		setA( A() + E() );
		break;
	case 0x84:
		setA( A() + H() );
		break;
    case 0x85:
		setA( A() + L() );
		break;
    case 0x86:
		setA( A() + read8( HL() ) );
		break;
    case 0x87:
		setA( A() + A() );
		break;
    case 0x88:
		setA( A() + B() + isCarr() );
		break;
    case 0x89:
		setA( A() + C() + isCarr() );
		break;
    case 0x8A:
		setA( A() + D() + isCarr() );
		break;
    case 0x8B:
		setA( A() + E() + isCarr() );
		break;
	case 0x8C:
		setA( A() + H() + isCarr() );
		break;
    case 0x8D:
		setA( A() + L() + isCarr() );
		break;
    case 0x8E:
		setA( A() + read8( HL() ) + isCarr() );
		break;
    case 0x8F:
		setA( A() + A() + isCarr() );
		break;
		
		
	case 0x90:
		setA( A() - B() );
		break;
    case 0x91:
		setA( A() - C() );
		break;
    case 0x92:
		setA( A() - D() );
		break;
    case 0x93:
		setA( A() - E() );
		break;
	case 0x94:
		setA( A() - H() );
		break;
    case 0x95:
		setA( A() - L() );
		break;
    case 0x96:
		setA( A() - read8( HL() ) );
		break;
    case 0x97:
		setA( A() - A() );
		break;
    case 0x98:
		setA( A() - B() - isCarr() );
		break;
    case 0x99:
		setA( A() - C() - isCarr() );
		break;
    case 0x9A:
		setA( A() - D() - isCarr() );
		break;
    case 0x9B:
		setA( A() - E() - isCarr() );
		break;
	case 0x9C:
		setA( A() - H() - isCarr() );
		break;
    case 0x9D:
		setA( A() - L() - isCarr() );
		break;
    case 0x9E:
		setA( A() - read8( HL() ) - isCarr() );
		break;
    case 0x9F:
		setA( A() - A() - isCarr() );
		break;
		
		
    }

}
