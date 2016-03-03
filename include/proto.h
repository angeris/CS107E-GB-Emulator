#include "uart.h"
int printf(char *,...);
void PUT32(unsigned,unsigned);

void reboot(void);
void system_enable_interrupts(void);
void system_disable_interrupts(void);

void set_cpsr(unsigned cpsr);
unsigned get_cpsr(void);
unsigned get_spsr(void);
char *bits_to_string(unsigned u); 

void enable_cache(void);
void disable_cache(void);
int in_critical_section(unsigned _pc);

typedef volatile long long cnt_t;
extern cnt_t a,b;
