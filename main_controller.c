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
	int x = getButtons();
    while(1) {
    	printf("x = %d\n", x);
    }
}
