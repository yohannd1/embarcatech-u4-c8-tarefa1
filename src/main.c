#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/timer.h"

#include "ssd1306.h"

#define JOYSTICK_X_PIN 27
#define JOYSTICK_X_INPUT 1

#define JOYSTICK_Y_PIN 26
#define JOYSTICK_Y_INPUT 0

#define LED_B_PIN 12
#define LED_R_PIN 13

static volatile uint pwm_b_slice;
static volatile uint pwm_r_slice;

static bool main_loop(struct repeating_timer *_);

int main(void) {
	stdio_init_all();

	gpio_init(LED_B_PIN);
	gpio_set_dir(LED_B_PIN, GPIO_OUT);

	gpio_init(LED_R_PIN);
	gpio_set_dir(LED_R_PIN, GPIO_OUT);

	adc_init();
	adc_gpio_init(JOYSTICK_X_PIN);
	adc_gpio_init(JOYSTICK_Y_PIN);

	// 30fps: 1s/30f = ~33ms/f
	struct repeating_timer timer;
	add_repeating_timer_ms(33, main_loop, NULL, &timer);

	while (true)
		sleep_ms(10000);

	return 0;
}

static bool main_loop(struct repeating_timer *_) {
	adc_select_input(JOYSTICK_X_INPUT);
	uint16_t x_axis = adc_read();

	adc_select_input(JOYSTICK_Y_INPUT);
	uint16_t y_axis = adc_read();

	return true;
}
