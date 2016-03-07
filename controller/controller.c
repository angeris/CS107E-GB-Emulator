#include "controller.h"
#include "SNESPad.h"
#include "gpio.h"
#include "proto.h"
#include "printf.h"
#include "timer.h"

const unsigned CLOCK = GPIO_PIN23;
const unsigned DATA = GPIO_PIN24;
const unsigned LATCH = GPIO_PIN25;

void controller_init(void) {
	gpio_set_function(LATCH, GPIO_FUNC_OUTPUT);
	gpio_set_function(CLOCK, GPIO_FUNC_OUTPUT);
	gpio_set_function(DATA, GPIO_FUNC_INPUT);
}

void latch(void) {
	gpio_write(LATCH, 1);
	gpio_write(LATCH, 0);
}

// Are you sure the clock on the controller is fast enough? Doesn't
// need interrupts or anything?
void clock_pulse(void) {
	gpio_write(CLOCK, 1);
	gpio_write(CLOCK, 0);
}

controller_state getState(void) {
	delay(10);

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
