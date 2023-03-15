#ifndef ADXL343_H
#define ADXL343_H

/*
     LSM6DS33-AN4682.pdf
		p8
 */

// register addresses
enum regAddr
{
      DEVID  = 0x00,

      THRESH_TAP        = 0x1D,
      OFSX              = 0x1E,
      OFSY              = 0x1F,
      OFSZ              = 0x20,
      DUR               = 0x21,
      LATENT            = 0x22,
      WINDOW            = 0x23,
      THRESH_ACT        = 0x24,
      THRES_INACT       = 0x25,
      TIME_INACT        = 0x26,
      ACT_INACT_CTL     = 0x27,
      THRESH_FF         = 0x28,
      TIME_FF           = 0x29,
      TAP_AXES          = 0x2A,
      ACT_TAP_STATUS    = 0x2B,
      BW_RATE           = 0x2C,
      POWER_CTL         = 0x2D,
      INT_ENABLE        = 0x2E,
      INT_MAP           = 0x2F,
      INT_SOURCE        = 0x30,
      DATA_FORMAT       = 0x31,
      DATAX0            = 0x32,
      DATAX1            = 0x33,
      DATAY0            = 0x34,
      DATAY1            = 0x35,
      DATAZ0            = 0x36,
      DATAZ1            = 0x37,
      FIFO_CTL          = 0x38,
      FIFO_STATUS       = 0x39
};

void adxl343_init();

void adxl343_write_reg(unsigned char reg, unsigned char v);
unsigned adxl343_read_reg(unsigned char reg);

unsigned adxl343_get_whoami(); 

void adxl343_enable_gyroscope();
void adxl343_read_gyroscope(short *x, short *y, short *z);

void adxl343_enable_accelerometer();
void adxl343_read_accelerometer(short *x, short *y, short *z);

#endif
