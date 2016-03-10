#include "MEM.h"
#include "CPU.h"
#include "proto.h"
#include "gpio.h"

void impossible_vector(unsigned pc) {
    printf("impossible exception at pc=%x\n", pc);
    reboot();
}

void interrupt_vector(unsigned pc) {
    return;
}

void main(void) {
    init_cpu();
    uart_init();
    while(1) {
       cpu_step(); // Should be done if there is no halting.
       //check interrupts:
       //if interrupt, then call correct given vector (unless GPU, then just call it whenever the interrupt occurs)
       //
       //check if waiting
    }
}
