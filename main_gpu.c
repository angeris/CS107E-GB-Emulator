#include "proto.h"
#include "gpio.h"
#include "GPU.h"
#include "CPU.h"
#include "MEM.h"
#include "timer.h"
#include "controller.h"

extern unsigned _gpu_mode;
controller_state gcs;

void impossible_vector(unsigned pc) {
    printf("impossible exception at pc=%x\n", pc);
    reboot();
}

void interrupt_vector(unsigned pc) {
    return;
}

void main(void) {
    delay(3);
    gpu_init();
    init_cpu();
    controller_init();
    while(1) {
        if(!gb_halt) cpu_step();
    	gpu_exec();
        if(getState().R) reboot();
    }
}
