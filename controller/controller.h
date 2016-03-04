#ifndef CONTROLLER_H
#define CONTROLLER_H

typedef struct {
	unsigned int B : 1;
	unsigned int Y : 1;
	unsigned int SELECT : 1;
	unsigned int START : 1;
	unsigned int UP : 1;
	unsigned int DOWN : 1;
	unsigned int LEFT : 1;
	unsigned int RIGHT : 1;
	unsigned int A : 1;
	unsigned int X : 1;
	unsigned int L : 1;
	unsigned int R : 1;
} controller_state;

void controller_init(void);
void latch(void);
void clock_pulse(void);
controller_state getState(void);

#endif
