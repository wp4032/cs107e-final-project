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

volatile static float pitch = 0.0;
volatile static float roll = 0.0;


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


// FUNCTION: accel_complementary_filter
// PARAMS: short *x, short *y, short *z
// RETURNS: uses a complmentary filter for sensor fusion of the acclerometer and gyroscope
// CITATION: https://github.com/seanboe/SimpleFusion/blob/master/src/simpleFusion.cpp
void accel_complementary_filter(float *angle_x, float *angle_y) {
    // Gets the pitch and roll from the accelerometer itself through some trigonometry
    float pitchAccel = atan2(accel_get_x(), sqrt(pwr(accel_get_y(), 2) + pwr(accel_get_z(), 2)));
    float rollAccel = atan2(accel_get_y(), sqrt(pwr(accel_get_x(), 2) + pwr(accel_get_z(), 2)));

    // Uses complementary filter to facilitate sensor fusion where we get the fast motions through the gyroscope while
    // the long lasting changes over a long period of time are from the acceleromter
    // Alpha - the biasing towards once sensor over another
    pitch = gyro_alpha * (pitch + ((-1 * gyro_get_y() / 1600.0) * (1.00 / refresh_rate))) + (1.00 - gyro_alpha) * pitchAccel;
    roll = gyro_alpha * (roll + ((gyro_get_x() / 1600.0) * (1.00 / refresh_rate))) + (1.00 - gyro_alpha) * rollAccel;

    // Once in radians, returns as degerees
    *angle_x = pitch * (180 / _pi);
    *angle_y = roll * (180 / _pi);
}


// FUNCTION: accel_print_angles
// PARAMS: void
// RETURNS: prints the pitch_x and roll_y angles on the accelerometer
void accel_print_angles(void) {
    float angle_x = 0.0;
    float angle_y = 0.0;

    accel_complementary_filter(&angle_x, &angle_y);

    // Did not implement %f in printf --> so just multiply by 1000 to see some of the digits in the decimal
    printf("pitch_x: %d, roll_y: %d\n", (int) (angle_x * 1000), (int) (angle_y * 1000));
}


// FUNCTION: accel_get_angles
// PARAMS: float *pitch_x, float *roll_y
// RETURNS: gets the accelerometer's pitch and roll angles and returns by variables called by reference
void accel_get_angles(float *pitch_x, float *roll_y) {
    accel_complementary_filter(pitch_x, roll_y);
}


// FUNCTION: accel_loop_angles
// PARAMS: void
// RETURNS: prints the pitch_x and roll_y angles over and over again
// used for testing the code
void accel_loop_angles(void) {
    while(1) {
        timer_delay_ms(10);
        accel_print_angles();
    }
}

