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
	controller_state cs = getState();
    while(1) {
    	printf("sizeof(cs) = %d\n", sizeof(cs));
    }
}
