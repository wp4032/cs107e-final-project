/*
  ADXL343 - Triple-Axis Accelerometer
    https://www.adafruit.com/product/4097

  Raspberry Pi I2C
	SDA = gpio pin2
	SCL = gpio pin3

    void i2c_init(void);
    void i2c_read(unsigned slave_address, char *data, int data_length);
    void i2c_write(unsigned slave_address, char *data, int data_length);

    i2c slave address:
      Below are the i2c addresses for https://www.analog.com/media/en/technical-documentation/data-sheets/adxl343.pdf
  	  ADXL343 (gyro and accelerometer)	0b0011101  0x1D
 */
#include "timer.h"
#include "printf.h"
#include "i2c.h"
#include "ADXL343.h"

const unsigned adxl343_address = 0b1010011; // this is for the gyro/accel;

void adxl343_write_reg(unsigned char reg, unsigned char v) {
	char data[2] = {reg, v};
	i2c_write(adxl343_address, data, 2);
}

unsigned adxl343_read_reg(unsigned char reg) {
	i2c_write(adxl343_address, (void*) &reg, 1);
	unsigned char uc = 0;
	i2c_read(adxl343_address, (void*) &uc, 1);
	return uc;
}

void adxl343_init() {
  printf("Init");
	adxl343_write_reg(BW_RATE, 0b1000);   // 1600Hz (high perf mode)
}

unsigned adxl343_get_whoami() {
	// should return 0x69 ...
  return adxl343_read_reg(DEVID);
}

// default is enabled
void adxl343_enable_accelerometer() {
    // accelerator _XL registers
	// adxl343_write_reg(POWER_CTL, 0x08);  // ACCEL: x,y,z enabled (bits 4-6)
  adxl343_write_reg(FIFO_CTL, 0b0100000);
  adxl343_write_reg(DATA_FORMAT, 0b100);
}

void adxl343_read_accelerometer(short *x, short *y, short *z) {
    *x =  adxl343_read_reg(DATAX0);

    *y =  adxl343_read_reg(DATAY0);

    *z =  adxl343_read_reg(DATAZ0);
}
