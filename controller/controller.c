#include "controller.h"
#include "SNESPad.h"
#include "gpio.h"
#include "printf.h"
#include "timer.h"
#include "interrupt.h"
#include "proto.h"

const unsigned CLOCK = GPIO_PIN23;
const unsigned DATA = GPIO_PIN24;
const unsigned LATCH = GPIO_PIN25;

#define RPI_INT_ENABLE_1             0x2000B210
#define RPI_INT_ENABLE_2             0x2000B214
#define RPI_INT_DISABLE_1            0x2000B21C
#define RPI_INT_DISABLE_2            0x2000B220

void controller_init(void) {
  	gpio_init();
  	printf("controller init\n");

	gpio_set_function(LATCH, GPIO_FUNC_OUTPUT);
	gpio_set_function(CLOCK, GPIO_FUNC_OUTPUT);
	gpio_set_function(DATA, GPIO_FUNC_INPUT);

  	// Ensure all interrupts are disabled. 
  	PUT32(RPI_INT_DISABLE_1, 0xFFFFFFFF);
  	PUT32(RPI_INT_DISABLE_2, 0xFFFFFFFF);

	// Enable Timer Interrupt 1
	PUT32(RPI_INT_ENABLE_1, 2); 

	// Enable interrupts
  	system_enable_interrupts(); 
}

void co_printtime(void) {
	printTime();
}

void latch(void) {
	gpio_write(LATCH, 1);
	gpio_write(LATCH, 0);
}

void clock_pulse(void) {
	gpio_write(CLOCK, 1);
	gpio_write(CLOCK, 0);
}

volatile int cnt = 0;
void controller_int_handler(unsigned pc) {
	cnt++;
	// printf("Controller Interrupt!\n");
	scheduleInterrupt();
	clearTimerInterrupt();
}

int getCount() {
	return cnt;
}

controller_state getState(void) {
	controller_state cs;

	latch();

	cs.B = !gpio_read(DATA);
	clock_pulse();
	cs.Y = !gpio_read(DATA);
	clock_pulse();
	cs.SELECT = !gpio_read(DATA);
	clock_pulse();
	cs.START = !gpio_read(DATA);
	clock_pulse();
	cs.UP = !gpio_read(DATA);
	clock_pulse();
	cs.DOWN = !gpio_read(DATA);
	clock_pulse();
	cs.LEFT = !gpio_read(DATA);
	clock_pulse();
	cs.RIGHT = !gpio_read(DATA);
	clock_pulse();
	cs.A = !gpio_read(DATA);
	clock_pulse();
	cs.X = !gpio_read(DATA);
	clock_pulse();
	cs.L = !gpio_read(DATA);
	clock_pulse();
	cs.R = !gpio_read(DATA);

	return cs;
}
