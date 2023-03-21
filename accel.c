#include "timer.h"
#include "uart.h"
#include "i2c.h"
#include "printf.h"
#include "LSM6DS33.h"
#include "assert.h"
#include "math.h"

// For complementary filter
static float refresh_rate = 100.0; // Hz
static float gyro_alpha = 0.98;

static float pitch = 0.0;
static float roll = 0.0;
static float yaw = 3.14/2.0;


// FUNCTION: accel_init
// PARAMS: void
// RETURNS: initializes I2C protocol and LSM6DS33 accelerometer
void accel_init(void) {
    i2c_init();
    lsm6ds33_init();

    assert(lsm6ds33_check_whoami());
}

// FUNCTION: accel_get_x
// PARAMS: void
// RETURNS: the acceleration in the x direction
// CITATION: from given LSM6DS33.c implementation 
signed short accel_get_x(void) {
    signed short x =  lsm6ds33_read_reg(OUTX_L_XL);
    x |= lsm6ds33_read_reg(OUTX_H_XL) << 8;
    return x;
}

// FUNCTION: accel_get_y
// PARAMS: void
// RETURNS: the acceleration in the y direction
// CITATION: from given LSM6DS33.c implementation 
signed short accel_get_y(void) {
    signed short y =  lsm6ds33_read_reg(OUTY_L_XL);
    y |= lsm6ds33_read_reg(OUTY_H_XL) << 8;
    return y; 
}

// FUNCTION: accel_get_z
// PARAMS: void
// RETURNS: the acceleration in the z direction
// CITATION: from given LSM6DS33.c implementation 
signed short accel_get_z(void) {
    signed short z =  lsm6ds33_read_reg(OUTZ_L_XL);
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
signed short gyro_get_x(void) {
    short x =  lsm6ds33_read_reg(OUTX_L_G);
    x |= lsm6ds33_read_reg(OUTX_H_G) << 8;
    return x;
}

// FUNCTION: gyro_get_y
// PARAMS: void
// RETURNS: the angular velocity in the y direction
// CITATION: from given LSM6DS33.c implementation 
signed short gyro_get_y(void) {
    short y =  lsm6ds33_read_reg(OUTY_L_G);
    y |= lsm6ds33_read_reg(OUTY_H_G) << 8;
    return y; 
}

// FUNCTION: gyro_get_z
// PARAMS: void
// RETURNS: the angular velocity in the z direction
// CITATION: from given LSM6DS33.c implementation 
signed short gyro_get_z(void) {
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


// FUNCTION: complementary_filter
// PARAMS: short *x, short *y, short *z
// RETURNS: uses a complmentary filter for sensor fusion of the acclerometer and gyroscope
// CITATION: https://github.com/seanboe/SimpleFusion/blob/master/src/simpleFusion.cpp
void accel_complementary_filter(float *angle_x, float *angle_y, float *angle_z) {
    int x = gyro_get_x();
    int y = gyro_get_y();
    int z = gyro_get_z();

    float pitchAccel = atan2(-1 * accel_get_x(), sqrt(pwr(accel_get_y(), 2) + pwr(accel_get_z(), 2)));
    float rollAccel = atan2(accel_get_y(), sqrt(pwr(accel_get_x(), 2) + pwr(accel_get_z(), 2)));
    float yawAccel = atan2(accel_get_z(), sqrt(pwr(accel_get_x(), 2) + pwr(accel_get_y(), 2)));
    // printf("x: %d, y: %d, z: %d\n", x/16, y/16, z/16);
    // printf("p: %d, r: %d, y: %d\n", (int) (pitchAccel * 100), (int) (rollAccel * 100), (int) (yawAccel * 100));
    // printf("p: %d, r: %d, y: %d\n", (int) ((_pi) * 1000), (int) ((_pi) * 1000), (int) ((_pi) * 1000));
    // printf("p: %d, r: %d, y: %d\n", (int) (pitchAccel * (180.0 / _pi)), (int) (rollAccel * (180.0 / _pi)), (int) (yawAccel * (180.0 / _pi)));

    pitch = gyro_alpha * (pitch + ((gyro_get_y() / 1600.0) * (1.00 / refresh_rate))) + (1.00 - gyro_alpha) * pitchAccel;
    roll = gyro_alpha * (roll + ((gyro_get_x() / 1600.0) * (1.00 / refresh_rate))) + (1.00 - gyro_alpha) * rollAccel;
    yaw = gyro_alpha * (yaw + ((gyro_get_z() / 1600.0) * (1.00 / refresh_rate))) + (1.00 - gyro_alpha) * yawAccel;
    // printf("pp: %d, rr: %d, yy: %d\n", (int) (pitch * 100), (int) (roll * 100), (int) (yaw * 100));

    *angle_x = pitch * (180 / _pi);
    *angle_y = roll * (180 / _pi);
    *angle_z = yaw * (180 / _pi);
}

void accel_print_angles(void) {
    float angle_x = 0.0;
    float angle_y = 0.0;
    float angle_z = 0.0;

    accel_complementary_filter(&angle_x, &angle_y, &angle_z);

    printf("theta_x: %d, theta_y: %d, theta_z: %d\n", (int) angle_x, (int) angle_y, (int) angle_z);
}

void accel_loop_angles(void) {
    while(1) {
        timer_delay_ms(10);
        accel_print_angles();
    }
}

