#ifndef ACCEL_H
#define ACCEL_H

/*
 * Interface for LSM6DS33 accelerometer and gyroscope for the 
 * DREAM'S DON'T DIE Moonwalker Brain Machine Interface
 * to have spatial awareness of the device
 *
 * Author: William Pan <williampan@cs.stanford.edu>
 * Date: Mar 24, 2023
 */


void accel_init();

// FUNCTIONS - Accelerometer
signed short accel_get_x(void);
signed short accel_get_y(void);
signed short accel_get_z(void);
void accel_xyz(short *x, short *y, short *z);

// FUNCTIONS - Gyroscope
signed short gyro_get_x();
signed short gyro_get_y();
signed short gyro_get_z();
void gyro_xyz(short *x, short *y, short *z);

// FUNCTIONS - Complementary Filter
void accel_complementary_filter(float *angle_x, float *angle_y, float *angle_z);
void accel_print_angles(void);
void accel_loop_angles(void);
void accel_get_angles(float *pitch_x, float *roll_y);



#endif
