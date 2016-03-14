#include "CPU.h"
#include "proto.h"

static int counter = 0;

static inline gb_long SP(); 
static inline gb_long PC(); 
static inline gb_long AF(); 
static inline gb_long BC(); 
static inline gb_long DE(); 
static inline gb_long HL(); 

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



// Mem access
gb_short cpu_read8() {
    return read8(_cpr.PC++);
}
gb_short_s cpu_read8s() {
    return read8s(_cpr.PC++);
}


gb_long cpu_read16() {
    gb_long t = read16(_cpr.PC);
	_cpr.PC += 2;
	return t;
}

// long register read/write
static inline void setBC(gb_long l) {
    _cpr.B = (gb_short)(l>>8);
    _cpr.C = (gb_short)(l&0xFF);
}

static inline void setAF(gb_long l) {
  _cpr.A = (gb_short)(l>>8);
  _cpr.F = (gb_short)(l&0xFF);
}

static inline void setDE(gb_long l) {
    _cpr.D = (gb_short)(l>>8);
    _cpr.E = (gb_short)(l&0xFF);
}

static inline void setHL(gb_long l) {
    _cpr.H = (gb_short)(l>>8);
    _cpr.L = (gb_short)(l&0xFF);
}

static inline void setSP(gb_long a) {
  _cpr.SP = a;
}

static inline void setPC(gb_long a) {
  _cpr.PC = a; 
}


void init_cpu() {
    setAF(0x01B0);
    setBC(0x0013);
    setDE(0x00D8);
    setHL(0x014D);
    setSP(0xFFFE);
    setPC(0x0100);
    write8(0xFF40, 0x91);
	write8(0xFF42, 0); // No need for the 0 writes.
	write8(0xFF43, 0);
	write8(0XFF45, 0);
	write8(0xFF47, 0XFC);
	write8(0xFF48, 0XFF);
	write8(0XFF49, 0XFF);
	write8(0xFF4A, 0);
	write8(0XFF4B, 0);
	write8(0xFFFF, 0);
}

// Read helpers (for clarity)
static inline gb_short A() { return _cpr.A; }
static inline gb_short F() { return _cpr.F; }
static inline gb_short B() { return _cpr.B; }
static inline gb_short C() { return _cpr.C; }
static inline gb_short D() { return _cpr.D; }
static inline gb_short E() { return _cpr.E; }
static inline gb_short H() { return _cpr.H; }
static inline gb_short L() { return _cpr.L; }
static inline gb_long SP() { return _cpr.SP; }
static inline gb_long PC() { return _cpr.PC; }
static inline gb_long AF() { return (gb_long)_cpr.A << 8 | (gb_long)_cpr.F; }
static inline gb_long BC() { return (gb_long)_cpr.B << 8 | (gb_long)_cpr.C; }
static inline gb_long DE() { return (gb_long)_cpr.D << 8 | (gb_long)_cpr.E; }
static inline gb_long HL() { return (gb_long)_cpr.H << 8 | (gb_long)_cpr.L; }

static inline void setA(gb_short a) { _cpr.A = a; }
static inline void setF(gb_short a) { _cpr.F = a; }
static inline void setB(gb_short a) { _cpr.B = a; }
static inline void setC(gb_short a) { _cpr.C = a; }
static inline void setD(gb_short a) { _cpr.D = a; }
static inline void setE(gb_short a) { _cpr.E = a; }
static inline void setH(gb_short a) { _cpr.H = a; }
static inline void setL(gb_short a) { _cpr.L = a; }

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
    setZero(!reg);
    setSubs(0);
    setHalf(!(reg & 0xf));
    return reg;
}
static gb_long inc16(gb_long reg) {
    return reg+1;
}
static gb_short dec8(gb_short reg) {
    reg--;
    setZero(!reg);
    setSubs(1);
    setHalf(!!(reg & 0xf));
    return reg;
}
static gb_long dec16(gb_long reg) {
    return reg-1;
}

static gb_short rot8_l(gb_short reg) {
    gb_short lbit = !!(reg & 1<<7);
    reg = reg<<1 | lbit; 
    setCarr(lbit);
    setZero(0);
    setHalf(0);
    setSubs(0);
    return reg;
}

static gb_short rot8_r(gb_short reg) {
    gb_short rbit = reg & 1;
    reg = (reg >> 1) | (rbit << 7);
    setCarr(rbit);
    setZero(0);
    setHalf(0);
    setSubs(0);
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
	setPC(_cpr.PC + read8s(_cpr.PC) + 1);
	
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
	setHalf((a&0xF) + (val&0xF) > 0xF);
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


static gb_short and8(gb_short val) {
	gb_short a = A();
	gb_short f = a & val;
	setZero(f == 0);
	setSubs(0);
	setHalf(1);
	setCarr(0);
	return f;
}


static gb_short xor8(gb_short val) {
	gb_short a = A();
	gb_short f = a ^ val;
	setZero(f == 0);
	setCarr(0);
	setSubs(0);
	setHalf(0);
	return f;
}


static gb_short or8(gb_short val) {
	gb_short f = A() | val;
	setZero(f == 0);
	setCarr(0);
	setSubs(0);
	setHalf(0);
	return f;
}


static gb_short cp8(gb_short val) {
	gb_short a = A();
	unsigned f = (unsigned)a - (unsigned)val;
	setZero(f == 0);
	setSubs(1);
	setCarr((gb_short)f > a);
	setHalf(0);
	return (gb_short)f;
}


static void incSP() {
	setSP( SP() + 2 );
}


static void incPC() {
	setPC( PC() + 2 );
}


static void decSP() {
	setSP( SP() - 2 );
}


static gb_long readSP() {
	return (gb_long)read8(SP()+1) | (gb_long)read8(SP()+2)<<8;
}


static void ret() {
	setPC( readSP() );
	incSP();
}


static void jump() {
	setPC( cpu_read16() );
}


static void push(gb_long val) {
	write8( SP(), (gb_short)(val>>8) );
    write8( SP()-1, (gb_short)(val) );
	decSP();
}

static void call() {
	push(PC()+2);
	setPC( cpu_read16() );
}


static void reset(gb_long val) {
	push(PC());
	setPC(val);
}

// Jump and link register
static inline void brlx(gb_long val) {
    reset(val);
}

void cpu_step() {
    unsigned _af = AF();
    unsigned _bc = BC();
    unsigned _de = DE();
    unsigned _hl = HL();
    unsigned _sp = SP();
    unsigned _pc = PC()+1;
    unsigned _opcode = read8(PC());
    
    // printf("AF:0x%04x,BC:0x%04x,DE:0x%04x,HL:0x%04x,SP:0x%04x,PC:0x%04x,OP:0x%02x\n", _af, _bc, _de, _hl, _sp, _pc, _opcode); 
    gb_short interrupts = read8(INT_FLAG);
    if(getIME()) {
        if(interrupts & INT_VBLANK)         { brlx(INT_VBLANK_ADDR);    interrupts &= ~INT_VBLANK;}
        else if(interrupts & INT_LCDSTAT)   { brlx(INT_LCDC_ADDR);      interrupts &= ~INT_LCDSTAT; }
        else if(interrupts & INT_TIMER)     { brlx(INT_TIMER_ADDR);     interrupts &= ~INT_TIMER; }
        else if(interrupts & INT_SERIAL)    { brlx(INT_SERIAL_ADDR);    interrupts &= ~INT_SERIAL; }
        else if(interrupts & INT_JOYPAD)    { brlx(INT_JOYPAD_ADDR);    interrupts &= ~INT_JOYPAD; }
    }
    exec_op(cpu_read8());
}

// CPU operations
void exec_op(gb_short op_code) {
//    if(counter++ > 40000) {
//        printf("We're done!");
//        reboot();
//        while(1); // Hang
//    }

    
    switch(op_code) {
    case 0x00:
      break;
    case 0x01:
      setBC(cpu_read16()); 
      break;
    case 0x02:
      write8(BC(), A());
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
      write16(cpu_read16(), SP());
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
      write8(DE(), A());
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
      write8(HL(), A());
      setHL( inc16( HL() ) );
      break;
    case 0x23:
      setHL( inc16( HL() ) );
      break;
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
      write8(HL(), A());
      setHL( dec16( HL() ) );
      break;
    case 0x33:
      setSP( inc16( SP() ) );
      break;
    case 0x34:
      write16(HL(), inc16( cpu_read16( HL() ) ) );
      break;
    case 0x35:
      write16(HL(), dec16( cpu_read16( HL() ) ) );
      break;
    case 0x36:
      write16(HL(), cpu_read8());
      break;
    case 0x37:
      //SCF();
      break;
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
      write8( HL(), B() );
      break;
    case 0x71:
      write8( HL(), C() );
      break;
    case 0x72:
      write8( HL(), D() );
      break;
    case 0x73:
      write8( HL(), E() );
      break;
    case 0x74:
      write8( HL(), H() );
      break;
    case 0x75:
      write8( HL(), L() );
      break;
    case 0x76:
      gb_halt = 1;
      break;
    case 0x77:
      write8( HL(), A() );
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
      setA( add8( B() ) );
      break;
    case 0x81:
      setA( add8( C() ) );
      break;
    case 0x82:
      setA( add8( D() ) );
      break;
    case 0x83:
      setA( add8( E() ) );
      break;
    case 0x84:
      setA( add8( F() ) );
      break;
    case 0x85:
      setA( add8( L() ) );
      break;
    case 0x86:
      setA( add8( read8( HL() ) ) );
      break;
    case 0x87:
      setA( add8( A() ) );
      break;
    case 0x88:
      setA( add8( B() + isCarr() ) );
      break;
    case 0x89:
      setA( add8( C() + isCarr() ) );
      break;
    case 0x8A:
      setA( add8( D() + isCarr() ) );
      break;
    case 0x8B:
      setA( add8( E() + isCarr() ) );
      break;
    case 0x8C:
      setA( add8( F() + isCarr() ) );
      break;
    case 0x8D:
      setA( add8( L() + isCarr() ) );
      break;
    case 0x8E:
      setA( add8( read8( HL() ) + isCarr() ) );
      break;
    case 0x8F:
      setA( add8( A() + isCarr() ) );
      break;
		
		
    case 0x90:
      setA( subtract8( B() ) );
      break;
    case 0x91:
      setA( subtract8( C() ) );
      break;
    case 0x92:
      setA( subtract8( D() ) );
      break;
    case 0x93:
      setA( subtract8( E() ) );
      break;
    case 0x94:
      setA( subtract8( F() ) );
      break;
    case 0x95:
      setA( subtract8( L() ) );
      break;
    case 0x96:
      setA( subtract8( read8( HL() ) ) );
      break;
    case 0x97:
      setA( subtract8( A() ) );
      break;
    case 0x98:
      setA( subtract8( B() + isCarr() ) );
      break;
    case 0x99:
      setA( subtract8( C() + isCarr() ) );
      break;
    case 0x9A:
      setA( subtract8( D() + isCarr() ) );
      break;
    case 0x9B:
      setA( subtract8( E() + isCarr() ) );
      break;
    case 0x9C:
      setA( subtract8( F() + isCarr() ) );
      break;
    case 0x9D:
      setA( subtract8( L() + isCarr() ) );
      break;
    case 0x9E:
      setA( subtract8( read8( HL() ) + isCarr() ) );
      break;
    case 0x9F:
      setA( subtract8( A() + isCarr() ) );
      break;
		
		
    case 0xA0:
      setA( and8( B() ) );
      break;
    case 0xA1:
      setA( and8( C() ) );
      break;
    case 0xA2:
      setA( and8( D() ) );
      break;
    case 0xA3:
      setA( and8( E() ) );
      break;
    case 0xA4:
      setA( and8( F() ) );
      break;
    case 0xA5:
      setA( and8( L() ) );
      break;
    case 0xA6:
      setA( and8( read8( HL() ) ) );
      break;
    case 0xA7:
      setA( and8( A() ) );
      break;
    case 0xA8:
      setA( xor8( B() ) );
      break;
    case 0xA9:
      setA( xor8( C() ) );
      break;
    case 0xAA:
      setA( xor8( D() ) );
      break;
    case 0xAB:
      setA( xor8( E() ) );
      break;
    case 0xAC:
      setA( xor8( F() ) );
      break;
    case 0xAD:
      setA( xor8( L() ) );
      break;
    case 0xAE:
      setA( xor8( read8( HL() ) ) );
      break;
    case 0xAF:
      setA( xor8( A() ) );
      break;
		
		
    case 0xB0:
      setA( or8( B() ) );
      break;
    case 0xB1:
      setA( or8( C() ) );
      break;
    case 0xB2:
      setA( or8( D() ) );
      break;
    case 0xB3:
      setA( or8( E() ) );
      break;
    case 0xB4:
      setA( or8( F() ) );
      break;
    case 0xB5:
      setA( or8( L() ) );
      break;
    case 0xB6:
      setA( or8( read8( HL() ) ) );
      break;
    case 0xB7:
      setA( or8( A() ) );
      break;
    case 0xB8:
      setA( cp8( B() ) );
      break;
    case 0xB9:
      setA( cp8( C() ) );
      break;
    case 0xBA:
      setA( cp8( D() ) );
      break;
    case 0xBB:
      setA( cp8( E() ) );
      break;
    case 0xBC:
      setA( cp8( F() ) );
      break;
    case 0xBD:
      setA( cp8( L() ) );
      break;
    case 0xBE:
      setA( cp8( read8( HL() ) ) );
      break;
    case 0xBF:
      setA( cp8( A() ) );
      break;	
	
	
    case 0xC0:
      if (!isZero()) {
	ret();
      }
      break;
    case 0xC1:
      setBC(readSP());
      incSP();
      break;
    case 0xC2:
      if (!isZero()) {
	jump();
      }
      else {
	incPC();
      }
      break;
    case 0xC3:
      jump();
      break;
    case 0xC4:
      if ( !isZero() ) {
	call();
      }
      else {
	incPC();
      }
      break;
    case 0xC5:
      push(BC());
      break;
    case 0xC6:
      add8( cpu_read8() );
      break;
    case 0xC7:
      reset(0x00);
      break;
    case 0xC8:
      if (isZero()) {
	ret();
      }
      break;
    case 0xC9:
      ret();
      break;
    case 0xCA:
      if (isZero()) {
	jump();
      }
      else {
	incPC();
      }
      break;
    case 0xCB:
      //prefix CB
      break;
    case 0xCC:
      if (isZero()) {
	call();
      }
      else {
	incPC();
      }
      break;
    case 0xCD:
      call();
      break;
    case 0xCE:
      setA( add8( cpu_read8() + isCarr() ) );
      break;
    case 0xCF:
      reset(0x08);
      break;
		
		
    case 0xD0:
      if (!isCarr()) {
	ret();
      }
      break;
    case 0xD1:
      setDE(readSP());
      incSP();
      break;
    case 0xD2:
      if (!isCarr()) {
	jump();
      }
      else {
	incPC();
      }
      break;
    case 0xD3:
      break;
    case 0xD4:
      if ( !isCarr() ) {
	call();
      }
      else {
	incPC();
      }
      break;
    case 0xD5:
      push(DE());
      break;
    case 0xD6:
      setA( subtract8( cpu_read8() ) );
      break;
    case 0xD7:
      reset(0x10);
      break;
    case 0xD8:
      if ( isCarr() ) {
	ret();
      }
      break;
    case 0xD9:
      ret();
      //ENABLE INTERRUPTS
      break;
    case 0xDA:
      if (isCarr()) {
	jump();
      }
      else {
	incPC();
      }
      break;
    case 0xDB:
      break;
    case 0xDC:
      if (isCarr()) {
	call();
      }
      else {
	incPC();
      }
      break;
    case 0xDD:
      break;
    case 0xDE:
      setA( subtract8( cpu_read8() + isCarr() ) );
      break;
    case 0xDF:
      reset(0x18);
      break;
		
		
    case 0xE0:
      write8(0xFF00 + cpu_read8(), A());
      break;
    case 0xE1:
      setHL(readSP());
      incSP();
      break;
    case 0xE2:
      write8( C(), A() );
      break;
    case 0xE3:
      break;
    case 0xE4:
      break;
    case 0xE5:
      push(HL());
      break;
    case 0xE6:
      setA( and8( cpu_read8() ) );
      break;
    case 0xE7:
      reset(0x20);
      break;
    case 0xE8:;
      unsigned a = (unsigned) SP();
      int b = read8s(PC());
      unsigned val = a + b;
      setZero(0);
      setSubs(0);
      setHalf((a&0xFFF) + (b&0xFFF) > 0xFFF);
      setCarr(val >= 0 && val <= 0xFF);
      setSP((gb_long)val);
      break;
    case 0xE9:
      setPC(read16(HL()));
      break;
    case 0xEA:
      write8(cpu_read16(), A());
      break;
    case 0xEB:
      break;
    case 0xEC:
      break;
    case 0xED:
      break;
    case 0xEE:
      setA( xor8( cpu_read8() ) );
      break;
    case 0xEF:
      reset(0x28);
      break;	
	
	
    case 0xF0:;
      setA( read8( 0xFF00 + cpu_read8()) );
      break;
    case 0xF1:
      setAF(readSP());
      incSP();
      break;
    case 0xF2:
      setA( read8( C() ) );
      break;
    case 0xF3:
      setIME(0);
      break;
    case 0xF4:
      break;
    case 0xF5:
      push(AF());
      break;
    case 0xF6:
      setA( or8( cpu_read8() ) );
      break;
    case 0xF7:
      reset(0x30);
      break;
    case 0xF8:;
      unsigned c = (unsigned) SP();
      int d = read8s(PC());
      unsigned val2 = c + d;
      setHL((gb_long)val2);
      setZero(0);
      setSubs(0);
      setHalf((c&0xFFF) + (d&0xFFF) > 0xFFF);
      setCarr(val2 >= 0 && val2 <= 0xFF);
      break;
    case 0xF9:
      setSP( HL() );
      break;
    case 0xFA:
      setA( read8( cpu_read16() ) );
      break;
    case 0xFB:
      setIME(1);
      break;
    case 0xFC:
      break;
    case 0xFD:
      break;
    case 0xFE:
      setA( cp8( cpu_read8() ) );
      break;
    case 0xFF:
      reset(0x38);
      break;	
    default:
      printf("Instruction %04x not implemented", op_code);
      break;
	
	
    }

}
