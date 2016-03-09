#include "proto.h"
#include "interrupt.h"

/* 
 * The program will be interrupted 
 * every INTDELAY milliseconds.
 *
 * 60 milliseconds is 60000 microseconds 
 * 60 milliseconds is 0.06 seconds
 * In interrupt will happen approximately 17 per second
 */
#define INTDELAY 60 
#define ONESECOND 1000000

#define CS 0x20003000 // System Timer Control and Status (Timer Base)
#define CLO 0x20003004 // System Timer Counter Lower 32 bits
#define C1 0x20003010 // System Timer Compare 1; corresponds to IRQ line 1.


/*
 * That is, to clear an interrupt set in C1,
 * software must write 0x20 to CS 
 */
void clearTimerInterrupt(void) {
	PUT32(CS, 0x20);
}

void scheduleInterrupt(void) {
	// PUT32(C1, *(volatile unsigned int *)CLO + (ONESECOND));
	PUT32(C1, *(volatile unsigned int *)CLO + (INTDELAY * 10000));
}

void printTime(void) {
	printf("time = %d\n", *(volatile unsigned int *)CLO);
}
