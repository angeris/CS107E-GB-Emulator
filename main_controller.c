#include "proto.h"
#include "gpio.h"
#include "controller/controller.h"

void impossible_vector(unsigned pc) {
    printf("impossible exception at pc=%x\n", pc);
    reboot();
}

void interrupt_vector(unsigned pc) {
    return;
}

void main(void) {
	controller_init();	
	controller_state cs;
    while(1) {
		cs = getState();
    	printf("sizeof(cs) = %d\n", sizeof(cs));
    	printf("cs.B = %d\n", cs.B);
		printf("cs.Y = %d\n", cs.Y);
		printf("cs.SELECT = %d\n", cs.SELECT);
		printf("cs.START = %d\n", cs.START);
		printf("cs.UP = %d\n", cs.UP);
		printf("cs.DOWN = %d\n", cs.DOWN);
		printf("cs.LEFT = %d\n", cs.LEFT);
		printf("cs.RIGHT = %d\n", cs.RIGHT);
		printf("cs.A = %d\n", cs.A);
		printf("cs.X = %d\n", cs.X);
		printf("cs.L = %d\n", cs.L);
		printf("cs.R = %d\n", cs.R);
		printf("---------------");
    }
}
