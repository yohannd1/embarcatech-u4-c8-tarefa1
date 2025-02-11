#include <stdio.h>
#include "pico/stdlib.h"

int main(void) {
	stdio_init_all();

	while (true)
		sleep_ms(10000);

	return 0;
}
