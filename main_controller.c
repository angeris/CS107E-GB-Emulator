#include "proto.h"
#include "gpio.h"
#include "timer.h"
#include "controller.h"

extern controller_state gcs;

void impossible_vector(unsigned pc) {
    printf("impossible exception at pc=%x\n", pc);
    reboot();
}

void interrupt_vector(unsigned pc) {
  	controller_int_handler(pc);
}

void printtest() {
    	printf("sizeof(cs) = %d\n", sizeof(gcs));
    	printf("cs.B = %d\n", gcs.B);
		printf("cs.Y = %d\n", gcs.Y);
		printf("cs.SELECT = %d\n", gcs.SELECT);
		printf("cs.START = %d\n", gcs.START);
		printf("cs.UP = %d\n", gcs.UP);
		printf("cs.DOWN = %d\n", gcs.DOWN);
		printf("cs.LEFT = %d\n", gcs.LEFT);
		printf("cs.RIGHT = %d\n", gcs.RIGHT);
		printf("cs.A = %d\n", gcs.A);
		printf("cs.X = %d\n", gcs.X);
		printf("cs.L = %d\n", gcs.L);
		printf("cs.R = %d\n", gcs.R);
		printf("---------------\n");
}

void main(void) {
	delay(3);
	controller_init();	
    while(1) {
    	delay(0.5);
    	printtest();
    }
}
