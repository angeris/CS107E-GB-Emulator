#include "MEM.h"
#include "CPU.h"
#include "proto.h"
#include "gpio.h"

void impossible_vector(unsigned pc) {
    printf("impossible exception at pc=%x\n", pc);
    reboot();
}

void interrupt_vector(unsigned pc) {
    gb_halt = 0;
    return;
}

void main(void) {
    init_cpu();
    uart_init();
    printf("Starting\n");
    for(int i=0; i<50000000; i++); // Flush time
    printf("Finished flushing\n");
    //while(1) {printf("WTF"); for(int i=0; i<1000000; i++);}
    while(1) {
       if(!gb_halt) cpu_step(); // Should be done if there is no halting.
       //check interrupts:
       //if interrupt, then call correct given vector (unless GPU, then just call it whenever the interrupt occurs)
       //
       //check if waiting
    }
}
