#ifndef INTERRUPT_H
#define INTERRUPT_H

/* 
 * Documentation:
 * http://embedded-xinu.readthedocs.org/en/latest/arm/rpi/BCM2835-System-Timer.html
 */

void clearTimerInterrupt(void);
void scheduleInterrupt(void);

#endif
