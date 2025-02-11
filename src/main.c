#include <stdio.h>
#include <math.h>

#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/timer.h"
#include "hardware/pwm.h"

#include "ssd1306.h"

#define JOYSTICK_X_PIN 27
#define JOYSTICK_X_INPUT 1

#define JOYSTICK_Y_PIN 26
#define JOYSTICK_Y_INPUT 0

#define JOYSTICK_CORRECTION -0.1 // evitar flicker na luz quando o joystick não está sendo usado
#define JOYSTICK_WRAP 4096

#define LED_B_PIN 12
#define LED_R_PIN 13

static volatile uint pwm_b_slice;
static volatile uint pwm_r_slice;
static volatile bool led_use_pwm = true;

static bool main_loop(struct repeating_timer *_);
static uint setup_pwm_for(uint pin);
static float calc_duty_cycle(uint16_t axis_value, float correction);

int main(void) {
	stdio_init_all();

	gpio_init(LED_B_PIN);
	gpio_set_dir(LED_B_PIN, GPIO_OUT);

	gpio_init(LED_R_PIN);
	gpio_set_dir(LED_R_PIN, GPIO_OUT);

	adc_init();
	adc_gpio_init(JOYSTICK_X_PIN);
	adc_gpio_init(JOYSTICK_Y_PIN);

	pwm_b_slice = setup_pwm_for(LED_B_PIN);
	pwm_r_slice = setup_pwm_for(LED_R_PIN);

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

	if (led_use_pwm) {
		float dc_r = calc_duty_cycle(x_axis);
		float dc_b = calc_duty_cycle(y_axis);

		pwm_set_gpio_level(LED_R_PIN, (uint16_t)(dc_r * JOYSTICK_WRAP));
		pwm_set_gpio_level(LED_B_PIN, (uint16_t)(dc_b * JOYSTICK_WRAP));
	} else {
		pwm_set_gpio_level(LED_B_PIN, 0);
		pwm_set_gpio_level(LED_R_PIN, 0);
	}

	return true;
}

static float calc_duty_cycle(uint16_t axis_value) {
	int32_t middle_centered = (int32_t)axis_value - 2048;
	float normalized = (float)middle_centered / 2048.0f;
	float absolute = fabsf(normalized);
	float corrected = (absolute + JOYSTICK_CORRECTION) / (1.0f + JOYSTICK_CORRECTION);
	return fmaxf(corrected, 0.0f);
}

static uint setup_pwm_for(uint pin) {
	gpio_set_function(pin, GPIO_FUNC_PWM);
	uint slice = pwm_gpio_to_slice_num(pin);

	pwm_config config = pwm_get_default_config();
	pwm_config_set_clkdiv(&config, 100.0f);
	pwm_config_set_wrap(&config, JOYSTICK_WRAP);
	pwm_init(slice, &config, true);

	pwm_set_gpio_level(pin, 0);

	return slice;
}
