#include "proto.h"
#include "interrupt.h"

/* 
 * The program will be interrupted 
 * every INTDELAY milliseconds.
 *
 * 60 milliseconds is 60000 microseconds 
 * 60 milliseconds is 0.06 seconds
 * In interrupt will happen approximately 17 times per second
 */
#define INTDELAY 60 

#define CS 0x20003000 // System Timer Control and Status (Timer Base)
#define CLO 0x20003004 // System Timer Counter Lower 32 bits
#define C1 0x20003010 // System Timer Compare 1; corresponds to IRQ line 1.

/*
 * To clear an interrupt set in C1,
 * software must write to the first bit of CS 
 */
void clear_timer_interrupt_1(void) {
	PUT32(CS, 0x00000002);
}

/*
 * Schedules an interrupt by writing the next time
 * to be interrupted to C1. This is done by taking 
 * the current time, located at the lower 32 bits
 * of the system timer and incrementing it by the
 * pre-defined interrup delay
 */
void schedule_timer_interrupt(void) {
	PUT32(C1, *(volatile unsigned int *)CLO + (INTDELAY * 10000));
}

/* Alternate  clear_timer_interrupt_1 Implementation */
/**
static unsigned int *CS1 = (unsigned int*)CS;
void clear_timer_interrupt_1(void) { *CS1 = 1 << 1;}
**/
