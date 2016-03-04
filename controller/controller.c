#include "controller.h"
#include "SNESPad.h"
#include "gpio.h"
#include "proto.h"
#include "printf.h"

#define LATCH GPIO_PIN3
#define CLOCK GPIO_PIN4
#define DATA GPIO_PIN5

controller_state cs = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void controller_init(void) {
	gpio_set_function(LATCH, GPIO_FUNC_OUTPUT);
	gpio_set_function(CLOCK, GPIO_FUNC_OUTPUT);
	gpio_set_function(DATA, GPIO_FUNC_INPUT);
}

void latch(void) {
	gpio_write(LATCH, 1);
	gpio_write(LATCH, 0);
}

void clock_pulse(void) {
	gpio_write(CLOCK, 1);
	gpio_write(CLOCK, 0);
}

controller_state getState(void) {
	return cs;
}
