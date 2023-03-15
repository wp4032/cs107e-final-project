#ifndef ACCEL_H
#define ACCEL_H

/*
 * Interface for LSM6DS33 accelerometer and gyroscope for the 
 * DREAM'S DON'T DIE Moonwalker Brain Machine Interface
 * to have spatial awareness of the device
 *
 * Author: William Pan <williampan@cs.stanford.edu>
 * Date: Mar 13, 2023
 */


void accel_init();

int accel_get_x();
int accel_get_y();
int accel_get_z();
void accel_xyz(short *x, short *y, short *z);


int gyro_get_x();
int gyro_get_y();
int gyro_get_z();
void gyro_xyz(short *x, short *y, short *z);

#endif
