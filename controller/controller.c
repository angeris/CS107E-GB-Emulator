#include "controller.h"
#include "SNESPad.h"
#include "gpio.h"
#include "printf.h"
#include "timer.h"
#include "interrupt.h"
#include "proto.h"

/* Global Controller State */
controller_state gcs;

const unsigned CLOCK = GPIO_PIN23;
const unsigned DATA = GPIO_PIN24;
const unsigned LATCH = GPIO_PIN25;

#define RPI_INT_ENABLE_1             0x2000B210
#define RPI_INT_ENABLE_2             0x2000B214
#define RPI_INT_DISABLE_1            0x2000B21C
#define RPI_INT_DISABLE_2            0x2000B220

/**
 * Initializes the controller by first disabling all interrupts,
 * then setting the gpio pins designated to Latch, Clock and Data
 * to output, output and input respectively. Timer interrupts are
 * then enabled, system interrupts are enabled and the first 
 * timer interrupt is then scheduled.
 *
 * Timer Interrupt 1 is located at IRQ 1. In order to enable it a 1 
 * must be written to the first bit of the RPI Interrupt Enable address.
 * ( 0x00000002 == 1 << 1 )
 *
 */
void controller_init(void) {
  	gpio_init();
  	printf("controller init\n");

  	// Ensure all interrupts are disabled. 
  	PUT32(RPI_INT_DISABLE_1, 0xFFFFFFFF);
  	PUT32(RPI_INT_DISABLE_2, 0xFFFFFFFF);
	
	gpio_set_output(LATCH);
	gpio_set_output(CLOCK);
	gpio_set_input(DATA);
	// gpio_detect_falling_edge(DATA);

	// Enable Timer Interrupt 1
	PUT32(RPI_INT_ENABLE_1, 0x00000002);

	// Enable interrupts
  	system_enable_interrupts();
	schedule_timer_interrupt(); 
}

/*
 * The controller data signal is latched onto
 * by sending a high immediately followed by a 
 * low output to the Latch pin.
 */
void latch(void) {
	gpio_write(LATCH, 1);
	gpio_write(LATCH, 0);
}

/*
 * Each data bit from the controller is recieved 
 * by first sending a high immediately followed by
 * a low output to the clock pin.
 */
void clock_pulse(void) {
	gpio_write(CLOCK, 1);
	gpio_write(CLOCK, 0);
}

void controller_int_handler(unsigned pc) {
	update_global_controller_state();
	clear_timer_interrupt_1();
	schedule_timer_interrupt();
}

/**
 * Returns a controller_state struct instance,
 * which contains the state of all 12 buttons
 * First the controller is latched onto,
 * then a high-low pulse is sent to the clock line.
 * Following this clock pulse, the data line is then 
 * indicating the state of a specific button.
 * 
 * The button signals are reversed to send 1 when
 * pressed and 0 when unpressed.
 */
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

void update_global_controller_state(void) {
	gcs = getState();
}

/* Alternate Timer Interrupt 1 init */
/**
static unsigned int *Enable_IRQs1 = (unsigned int*)0x2000B210;
*Enable_IRQs1 = 1 << 1;
**/
