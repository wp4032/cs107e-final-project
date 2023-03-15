#include "timer.h"
#include "uart.h"
#include "i2c.h"
#include "printf.h"

#include "ADXL343.h"

void main(void) {

    timer_init();
	uart_init();

    i2c_init();
	adxl343_init();

    printf("Starting\n");
    printf("whoami=%x\n", adxl343_get_whoami());

	while(1) { 
        short x, y, z;
        adxl343_read_accelerometer(&x, &y, &z);
        // 16384 is 1g (1g == 1000mg)
        printf("accel=(%dmg,%dmg,%dmg)\n", x, y, z);
        timer_delay_ms(200);
	}
}

