#ifndef INTERRUPT_H
#define INTERRUPT_H

/* 
 * Documentation:
 * http://embedded-xinu.readthedocs.org/en/latest/arm/rpi/BCM2835-System-Timer.html
 */

void clear_timer_interrupt_1(void);
void schedule_timer_interrupt(void);

#endif
