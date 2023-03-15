#include "timer.h"
#include "uart.h"
#include "i2c.h"
#include "printf.h"
#include "LSM6DS33.h"
#include "assert.h"

void accel_init(void) {
    i2c_init();
    lsm6ds33_init();

    assert(lsm6ds33_check_whoami());
}

// FUNCTION: accel_get_x
// PARAMS: void
// RETURNS: the acceleration in the x direction
// CITATION: from given LSM6DS33.c implementation 
int accel_get_x(void) {
    short x =  lsm6ds33_read_reg(OUTX_L_XL);
    x |= lsm6ds33_read_reg(OUTX_H_XL) << 8;
    return x;
}

// FUNCTION: accel_get_y
// PARAMS: void
// RETURNS: the acceleration in the y direction
// CITATION: from given LSM6DS33.c implementation 
int accel_get_y(void) {
    short y =  lsm6ds33_read_reg(OUTY_L_XL);
    y |= lsm6ds33_read_reg(OUTY_H_XL) << 8;
    return y; 
}

// FUNCTION: accel_get_z
// PARAMS: void
// RETURNS: the acceleration in the z direction
// CITATION: from given LSM6DS33.c implementation 
int accel_get_z(void) {
    short z =  lsm6ds33_read_reg(OUTZ_L_XL);
    z |= lsm6ds33_read_reg(OUTZ_H_XL) << 8;
    return z; 
}

// FUNCTION: accel_xyz
// PARAMS: short *x, short *y, short *z
// RETURNS: xyz values of acceleration in pass through variable pointers
void accel_xyz(short *x, short *y, short *z) {
    *x = accel_get_x();
    *y = accel_get_y();
    *z = accel_get_z();
}

// FUNCTION: gyro_get_x
// PARAMS: void
// RETURNS: the angular velocity in the x direction
// CITATION: from given LSM6DS33.c implementation 
int gyro_get_x(void) {
    short x =  lsm6ds33_read_reg(OUTX_L_G);
    x |= lsm6ds33_read_reg(OUTX_H_G) << 8;
    return x;
}

// FUNCTION: gyro_get_y
// PARAMS: void
// RETURNS: the angular velocity in the y direction
// CITATION: from given LSM6DS33.c implementation 
int gyro_get_y(void) {
    short y =  lsm6ds33_read_reg(OUTY_L_G);
    y |= lsm6ds33_read_reg(OUTY_H_G) << 8;
    return y; 
}

// FUNCTION: gyro_get_z
// PARAMS: void
// RETURNS: the angular velocity in the z direction
// CITATION: from given LSM6DS33.c implementation 
int gyro_get_z(void) {
    short z =  lsm6ds33_read_reg(OUTZ_L_G);
    z |= lsm6ds33_read_reg(OUTZ_H_G) << 8;
    return z; 
}


// FUNCTION: gyro_xyz
// PARAMS: short *x, short *y, short *z
// RETURNS: xyz values of gyroscope in pass through variable pointers
void gyro_xyz(short *x, short *y, short *z) {
    *x = gyro_get_x();
    *y = gyro_get_y();
    *z = gyro_get_z();
}

void main(void) {


	// while(1) { 
    //     short x, y, z;
    //     lsm6ds33_read_accelerometer(&x, &y, &z);
    //     // 16384 is 1g (1g == 1000mg)
    //     printf("accel=(%dmg,%dmg,%dmg)\n", x/16, y/16, z/16);
    //     timer_delay_ms(200);
	// }
}

