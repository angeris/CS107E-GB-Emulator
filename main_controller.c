#include "proto.h"
#include "gpio.h"
#include "timer.h"
#include "controller/controller.h"

controller_state cs;

void impossible_vector(unsigned pc) {
    printf("impossible exception at pc=%x\n", pc);
    reboot();
}

void interrupt_vector(unsigned pc) {
  	controller_int_handler(pc);
}

void main(void) {
	delay(5);
	controller_init();	
    while(1) {
    	delay(2);
    	co_printtime();
    	printf("count = %d\n", getCount());
    }
}

void printtest() {
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
